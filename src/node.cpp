// The node
// TODO rework initialization
#include "node.h"

// Creates a Node object
// Reads all addresses and in a seperate list all neighbors
// reason to know all addresses is the exit
// TODO rewrite constructor with initial values
Node::Node(char* id_cstr) : vtime_terminated(false), responded(true){

	// Args get passed as cstring so I am converting first to String and then to int
	std::string id_str(id_cstr);

	// init starting values
	myid = std::stoi(id_str);
	clear_stringstream(ss);

	//----Read File
	// book knows all addresses because it is easier to terminate them all with one signal
	book = Addressbook(ADDRESSFILE);

	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	neighbors = Addressbook(ADDRESSFILE, get_nb_ids(GRAPHFILE, myid));

	// resize the vector to the size of the addressbook
	vtime.resize(book.entrycount());
	vtime_to_terminate.resize(book.entrycount());
	int vtime_to_respond = INIT_TERM_TIME;
	// for the init fill all values with 0 otherwise unexpoected behavior can happen
	std::fill(vtime.begin(),vtime.end(),0);
	std::fill(vtime_to_terminate.begin(),vtime_to_terminate.end(),INIT_TERM_TIME);

}

// returns a random value in the given range
unsigned int Node::get_random(const unsigned int& max, unsigned int min){

	if (min > max)
		return 0;

	if(max <= 0)
		return 0;

	if(min <= 0)
		min = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min,max);
	return dis(gen);
}

std::list<int> Node::get_candidate_ids(const std::string& fname){
	std::ifstream ifile;
	ifile.open(fname);
	std::list<int> candidate_ids;
	std::string line;
	int c_id;

	while(getline(ifile,line)){
		c_id = std::stoi(line);
		candidate_ids.push_back(c_id);
	}
	ifile.close();
	return candidate_ids;
}

// Message handling
int Node::logger_signal_out(Entry& receiver, Message& message, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << myid << " ]";
	ss << "[MType: OUT]";
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[V_TIME: " << vtime[myid - 1] << " ]";
	ss << "[Recv IP/Port: " << receiver.getip() << "/" << receiver.getport() << " ]";
	ss << message.get_msg();
	ss << "[Send: ";

	if (connection) {
		ss << "SUCCESS]";
	} else {
		ss << "FAILED]";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, 0, 0, ss.str()));
		logger.close_connection();
	} else {
		ss << "[LoggerCon FAILED]";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::logger_signal_in(Message& message){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << myid << " ]";
	ss << "[MType: IN]";
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[V_TIME: " << vtime[myid - 1] << " ]";
	ss << "[SenderID: " << message.get_sender().getid() << " ]";
	ss << message.get_msg();

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, 0, 0, ss.str()));
		logger.close_connection();
	} else {
		ss << "[LoggerCon FAILED]";
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

// Sends a signal to all addresses in the given book
int Node::send_all_signal(Addressbook receivers, int signalid){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		send_signal((*it), signalid);
	}while(++it != receivers.get_end());
	return -1;
}

// Sends a signal to the given Entry
int Node::send_signal(Entry& receiver, const int& signalid){
	vtime_up(vtime);
	Sender sender(receiver.getip(),receiver.getport());
	Message message(myself, signalid, 0, 0, "");
	message.set_vtimestamp(vtime);
	if(sender.get_connection() != -1){
		// TODO rework message class
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
	} else {
		logger_signal_out(receiver,message,false);
	}
	vtime_up(vtime);
	return -1;
}

int Node::send_all_message(Addressbook& receivers, Message& message){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		send_message((*it), message);
	}while(++it != receivers.get_end());
	return -1;
}

int Node::send_message(Entry& receiver, Message& message){
	vtime_up(vtime);
	message.set_vtimestamp(vtime);
	Sender sender(receiver.getip(),receiver.getport());
	if(sender.get_connection() != -1){
		// TODO rework message class
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
		return 0;
	} else {
		logger_signal_out(receiver,message,false);
	}
	return -1;
}

int Node::forward_all_message(Addressbook& receivers, Message& message){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		if ((*it).getid() != message.get_sender().getid())
			forward_message((*it), message);
	}while(++it != receivers.get_end());
	return -1;
}

int Node::forward_message(Entry& receiver, Message& message){
	Sender sender(receiver.getip(),receiver.getport());
	if(sender.get_connection() != -1){
		// TODO rework message class
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
	} else {
		logger_signal_out(receiver,message,false);
	}
	vtime_up(vtime);
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
	//	std::cout << "ID: " << myid << " IDs neighboring me:";
	//	// TODO eliminate duplicates
	//	for(auto v : ids_neighboring_me)
	//		std::cout << " " << v;
	//	std::cout << std::endl;
	return ids_neighboring_me;
}

// Return the current vectorclock array
std::vector<int> Node::get_vectortime(){
	return vtime;
}

// Switch case methods

// Case EXIT_NODE
int Node::sc_exit_node(Message& message, bool& listen_more){
	logger_signal_in(message);
	listen_more = false;
	return -1;
}

// Case EXIT_ALL
int Node::sc_exit_all(Message& message, bool& listen_more){
	logger_signal_in(message);
	Message new_message(message.get_sender(),EXIT_NODE,message.get_origin(), message.get_sender_clvl(),message.get_msg());
	send_all_message(book, new_message);
	listen_more = false;
	return -1;
}

// Case PRINT_VTIME
int Node::sc_print_vtime(){
	// TODO make it logger compatible
	for (unsigned int i = 0; i < vtime.size(); i++){
		std::cout << vtime[i] << " ";
	}
	std::cout << std::endl;
	return -1;
}

int Node::process_echo_explore(Message& explore){

	// if echo_identifier[explorer.get_msg_id()] == empty
	if (echo_identifier.count(explore.get_msg_id()) == 0){
	// 	create new struct
	// 	insert in map
		echo_identifier[explore.get_msg_id()] = Echo_content(); //Echo_content is a struct
	}

	// Struct current = map[msg_id]
	Echo_content current = echo_identifier[explore.get_msg_id()];
	// all following but with ref to the cur struct

	current.echo_counter++;
	// if state == white
	if (current.state == white){
		current.state = red;
		// send explore to neighbors (except sender)
		Message new_explore(myself, ECHO_EXPLORE, myself.getid(), 100, "");
		new_explore.set_msg_id(explore.get_msg_id());
		std::list<Entry>::iterator it = neighbors.get_iterator();
		do{
			if ((*it).getid() != explore.get_sender().getid())
				send_message((*it), new_explore);
		}while(++it != neighbors.get_end());
		// remember sender
		current.first_neighbor = explore.get_sender();
	}
	if (current.echo_counter == neighbors.entrycount()){
		current.state = green;
		Message echo(myself, ECHO_EXPLORE, myself.getid(), 100, "");
		echo.set_msg_id(explore.get_msg_id());
		send_message(current.first_neighbor, echo);
	}
	return -1;
}

// Count the vectortime up
int Node::vtime_up(std::vector<int>& vtimestamp){
	// -1 for offset
	vtime[myid - 1] = vtime[myid -1] + 1;

	// doesnt need offset
	// sets the current vtime to the max of current and received vtime
	for (unsigned int i = 0; i < vtime.size(); i++){
		vtime[i] = std::max(vtime[i], vtimestamp[i]);
	}
	return -1;
}

int Node::vtime_check_terminate(std::vector<int>& cur_vtime, std::vector<int>& term_vtime, bool& vtime_terminated){
	//if (cur_vtime >= term_vtime)
	if (cur_vtime[myid - 1] >= term_vtime[myid - 1]){
		if (!vtime_terminated){
			// TODO no time to explain, get to da chopper
			Message term_me(myself,NODE_TERMINATED,-1,-1,"");
			logger_signal_out(myself, term_me, true); 
		}
		vtime_terminated = true;
	}
	return -1;
}

int Node::set_termination_vtime(int term_time){
	std::fill(vtime_to_terminate.begin(),vtime_to_terminate.end(),term_time);
	return -1;
}

// The main loop of the node
// do until receive the exit signal
int Node::run(){

	// Lookup the id from argv and get my associated port
	// TODO rework this output
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "My ID is " << myid << ", my port is: " << myself.getport() << std::endl;


	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop

	// Init values
	int confd = -1;
	bool listen_more = true;

	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		Message message(Entry(-1,"",-1), -1, -1, -1, "");

		read(confd,&message,sizeof(message));


		// Vectortimestamp from active connection
		std::vector<int> vtimestamp;
		vtimestamp.resize(book.entrycount());
		std::fill(vtimestamp.begin(),vtimestamp.end(),0);

		switch(message.get_signal_id()){

			case EXIT_NODE : {
						 // exit single node
						 vtime_up(vtimestamp);
						 sc_exit_node(message, listen_more);
						 break;
					 }
			case EXIT_ALL : {
						// exit all nodes
						vtime_up(vtimestamp);
						sc_exit_all(message, listen_more);
						break;
					}
			case PRINT_VTIME : {
						   sc_print_vtime();
						   break;
					   }

			case SET_TERMINATE_VTIME : {
						vtime_up(vtimestamp);
						   set_termination_vtime(message.get_sender_clvl());
						   break;
					   }
			case SET_TERMINATE_VTIME_ALL : {
						vtime_up(vtimestamp);
						   set_termination_vtime(message.get_sender_clvl());
						send_all_message(book, message);
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
