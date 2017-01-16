// The node
// TODO rework initialization
// TODO get better string handling on cout
#include "node.h"

// Creates a Node object
// Reads all addresses and in a seperate list all neighbors
// reason to know all addresses is the exit
Node::Node(char* id_cstr){

	// Args get passed as cstring so I am converting first to String and then to int
	std::string id_str(id_cstr);

	// init starting values
	myid = std::stoi(id_str);
	heard_rumor = false;
	believe_rumor = false;
	clear_stringstream(ss);

	//----Read File
	// book knows all addresses because it is easier to terminate them all with one signal
	book = Addressbook(ADDRESSFILE);

	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	neighbors = Addressbook(ADDRESSFILE, get_nb_ids(GRAPHFILE, myid));

	// resize the vector to the size of the addressbook
	vtime.resize(book.entrycount());
	// for the init fill all values with 0
	std::fill(vtime.begin(),vtime.end(),0);
}

// Message handling
int Node::msg_out(std::list<Entry>::iterator& it, const std::string& msg, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	std::time_t t = std::time(nullptr);

	ss << "NODE_ID: " << myid;
	ss << "\n\t";
	ss << std::put_time(std::localtime(&t), "Timestamp: %H:%M:%S");
	ss << "\n\t";
	ss << "Message Type: OUT";
	ss << "\n\t";
	ss << "Receiver IP/Port: " << (*it).getip() << "/" << (*it).getport();
	ss << "\n\t";
	ss << "String sended: >>\"" << msg << "\"<<";
	ss << "\n\t";
	ss << "Sending status: ";

	if (connection) {
		ss << "SUCCESS";
		ss << "\n\t";
	} else {
		ss << "FAILED";
		ss << "\n\t";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_msg(ss.str());
		logger.close_connection();
	} else {
		ss << "Connection to logger failed.";
		ss << "\n\t";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::msg_out(const std::string& ip, const int& port, const std::string& msg, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	std::time_t t = std::time(nullptr);

	ss << "NODE_ID: " << myid;
	ss << "\n\t";
	ss << std::put_time(std::localtime(&t), "Timestamp: %H:%M:%S");
	ss << "\n\t";
	ss << "Message Type: OUT";
	ss << "\n\t";
	ss << "Receiver IP/Port: " << ip << "/" << port;
	ss << "\n\t";
	ss << "String sended: >>\"" << msg << "\"<<";
	ss << "\n\t";
	ss << "Sending status: ";

	if (connection) {
		ss << "SUCCESS";
		ss << "\n\t";
	} else {
		ss << "FAILED";
		ss << "\n\t";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_msg(ss.str());
		logger.close_connection();
	} else {
		ss << "Connection to logger failed.";
		ss << "\n\t";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::signal_out(std::list<Entry>::iterator& it,int& signalid,const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	std::time_t t = std::time(nullptr);

	ss << "NODE_ID: " << myid;
	ss << "\n\t";
	ss << std::put_time(std::localtime(&t), "Timestamp: %H:%M:%S");
	ss << "\n\t";
	ss << "Message Type: OUT";
	ss << "\n\t";
	ss << "Receiver IP/Port: " << (*it).getip() << "/" << (*it).getport();
	ss << "\n\t";
	ss << "Signal ID: " << signalid;
	ss << "\n\t";
	ss << "Sending status: ";

	if (connection) {
		ss << "SUCCESS";
		ss << "\n\t";
	} else {
		ss << "FAILED";
		ss << "\n\t";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_msg(ss.str());
		logger.close_connection();
	} else {
		ss << "Connection to logger failed.";
		ss << "\n\t";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::signal_in(const int& signalid){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	std::time_t t = std::time(nullptr);

	ss << "NODE_ID: " << myid;
	ss << "\n\t";
	ss << std::put_time(std::localtime(&t), "Timestamp: %H:%M:%S");
	ss << "\n\t";
	ss << "Message Type: IN";
	ss << "\n\t";
	ss << "Signal ID: " << signalid;
	ss << "\n\t";

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_msg(ss.str());
		logger.close_connection();
	} else {
		ss << "Connection to logger failed.";
		ss << "\n\t";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::msg_in(const int& signalid, const std::string& msg){
	
	Sender logger(LOGGER_IP, LOGGER_PORT);

	std::time_t t = std::time(nullptr);

	ss << "NODE_ID: " << myid;
	ss << "\n\t";
	ss << std::put_time(std::localtime(&t), "Timestamp: %H:%M:%S");
	ss << "\n\t";
	ss << "Message Type: IN";
	ss << "\n\t";
	ss << "Signal ID: " << signalid;
	ss << "\n\t";
	ss << "Content:";
	ss << "\n\t";
	ss << msg;
	ss << "\n\t";

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_msg(ss.str());
		logger.close_connection();
	} else {
		ss << "Connection to logger failed.";
		ss << "\n\t";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;

}

// Deletes the content in the stringstream
int Node::clear_stringstream(std::stringstream& ss){
	ss.str(std::string());
	ss.clear();
	return -1;
}

// Sends a string to all addresses in the given book
int Node::send_all_msg(Addressbook receivers, std::string msg){

	// Iterate over all entries in the addressbook
	// - build up a connection
	// - send the stringmsg
	// - send the logging message to the logger on succesful connection
	// - print the logging message to cout on unsuccessful logger connection
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{


		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_msg(vtime, msg);
			sender.close_connection();

			Node::msg_out(it,msg,true);
		} else {
			Node::msg_out(it,msg,false);
		}

	}while(++it != receivers.get_end());
	return -1;
}

// Sends a signal to all addresses in the given book
int Node::send_all_signal(Addressbook receivers, int signalid){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_signalid(signalid);
			Node::signal_out(it,signalid,true);
			sender.close_connection();
		} else {
			Node::signal_out(it,signalid,false);
		}

	}while(++it != receivers.get_end());
	return -1;
}

// Based on the Addressfile and the graphfile,
// create the Addressbook only with known neighbors
// TODO rework method, especially the cout
std::list<int> Node::get_nb_ids(std::string gfname, int own_id){
	std::ifstream gifile; // graph input file
	gifile.open(gfname);

	std::string line;
	std::list<int> ids_neighboring_me;
	//ids_neighboring_me.push_back(own_id); // I neighbor myself // why? //because inf loop Oo

	std::string idstrA, sep, idstrB, end;
	int idA, idB;

	getline(gifile,line); // Ignore first line
	while (getline(gifile,line)){
		std::stringstream iss(line);
		getline(iss,idstrA,' ');
		if(idstrA != "}"){
			getline(iss,sep,' ');
			getline(iss,idstrB,';');
			idA = std::stoi(idstrA);
			idB = std::stoi(idstrB);
			if(idA == own_id)
				ids_neighboring_me.push_back(idB);
			if(idB == own_id)
				ids_neighboring_me.push_back(idA);
		}
	}
	gifile.close();
	std::cout << "ID: " << myid << " IDs neighboring me:";
	// TODO eliminate duplicates
	for(auto v : ids_neighboring_me)
		std::cout << " " << v;
	std::cout << std::endl;
	return ids_neighboring_me;
}

// Return the current vectorclock array
std::vector<int> Node::get_vectortime(){
	return vtime;
}

// Switch case methods

// Case EXIT_NODE
int Node::sc_exit_node(bool& listen_more){
	Node::signal_in(EXIT_NODE);
	listen_more = false;
	return -1;
}

// Case EXIT_ALL
int Node::sc_exit_all(bool& listen_more){
	Node::signal_in(EXIT_ALL);
	send_all_signal(book,EXIT_NODE);
	listen_more = false;
	return -1;
}

// Case RECV_MSG
int Node::sc_recv_msg(int& confd){
	char a[MSG_BUFFER_SIZE];
	memset(&a[0],0,sizeof(a));
	read(confd,&a,sizeof(a));
	std::string msg_str(a);
	Node::msg_in(RECV_MSG,msg_str);
	return -1;
}

// Case SOCIALISE
int Node::sc_socialise(){
	// if there is no msg send in socialise, look up here, the ss.str() seems to be empty
	send_all_msg(neighbors, ss.str());

	return -1;
}


// Case PRINT_VTIME
int Node::sc_print_vtime(){
	for (int i = 0; i < vtime.size(); i++){
		std::cout << vtime[i] << " ";
	}
	std::cout << std::endl;
	return -1;
}

// Count the vectortime up
int Node::vtime_up(std::vector<int>& vtimestamp){
	vtime[myid - 1] = vtime[myid -1] + 1;
	for (int i = 0; i < vtime.size(); i++){
		vtime[i] = std::max(vtime[i], vtimestamp[i]);
	}
}

// The main loop of the node
// do until receive the exit signal
int Node::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "My ID is " << myid << ", my port is: " << myself.getport() << std::endl << "I believe a rumor if heard " << believe_border << " times." << std::endl;


	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop

	// Init values
	int confd = -1;
	bool listen_more = true;
	// The rumor counter has to be initialized, unexpected behaviour is the result otherwise
	rumor_counter = 0;

	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		int msg_id = -1;

		// Read the msgid from an active connection
		read(confd,&msg_id,sizeof(msg_id));

		// Vectortimestamp from active connection
		std::vector<int> vtimestamp;
		vtimestamp.resize(book.entrycount());
		std::fill(vtimestamp.begin(),vtimestamp.end(),0);

		// TODO Thats so unsafe its insane...
		for(int i = 0; i < vtimestamp.size(); i++){
			read(confd,&vtimestamp[i],sizeof(int));
		}

		switch(msg_id){

			case EXIT_NODE : {
						 // exit single node
						 vtime_up(vtimestamp);
						 sc_exit_node(listen_more);
						 break;
					 }
			case EXIT_ALL : {
						// exit all nodes
						vtime_up(vtimestamp);
						sc_exit_all(listen_more);
						break;
					}
			case RECV_MSG : {
						// recv msgs with max length of 256 chars
						// TODO check on length
						vtime_up(vtimestamp);
						sc_recv_msg(confd);
						break;
					}
			case SOCIALISE : {
						 // send a string msg to all my neighbors with my id
						 vtime_up(vtimestamp);
						 sc_socialise();
						 break;
					 }
			case RUMOR : {
					     // start spreading a rumor
					     vtime_up(vtimestamp);
					     sc_rumor(confd);
					     break;
				     }
			case PRINT_VTIME : {
						   sc_print_vtime();
						   break;
					   }

			default :
					   std::cout << "ID: " << myid << "I don't know this signal id. Close connection.\n";
					   break;
		}
		close(confd);

	}while(listen_more);
	std::cout << "ID: " << myid << " Node exited." << std::endl;
	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}
