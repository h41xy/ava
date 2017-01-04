// The node
// TODO rework initialization
// TODO get better string handling on cout
#include "node.h"

// Creates a Node object
// Reads all addresses and in a seperate list all neighbors
// reason to know all addresses is the exit
Node::Node(char* id_cstr){
	heard_rumor = false;
	believe_rumor = false;

	std::string id_str(id_cstr);
	myid = std::stoi(id_str);
	//----Read File
	book = Addressbook(ADDRESSFILE);
	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	neighbors = Addressbook(ADDRESSFILE, get_nb_ids(GRAPHFILE, myid));

	// initialise vector clock
	// resize the vector to the size of the addressbook
	vtime.resize(book.entrycount());
	// fill all values with 0
	std::fill(vtime.begin(),vtime.end(),0);
}

// Delegating constructors is c++11 only, I want to stay away from that
// so it is the same contsructor but the beliveing border parameter can be set
Node::Node(char* id_cstr, char* belive_border_cstr){
	heard_rumor = false;
	believe_rumor = false;

	std::string believe_border_str(belive_border_cstr);
	believe_border = std::stoi(believe_border_str);
	std::string id_str(id_cstr);
	myid = std::stoi(id_str);
	//----Read File
	book = Addressbook(ADDRESSFILE);
	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	neighbors = Addressbook(ADDRESSFILE, get_nb_ids(GRAPHFILE, myid));

	// initialise vector clock
	// resize the vector to the size of the addressbook
	vtime.resize(book.entrycount());
	// fill all values with 0
	std::fill(vtime.begin(),vtime.end(),0);
}

// Sends a string to all addresses in the given book
int Node::send_all_msg(Addressbook receivers, std::string msg){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_msg(vtime, msg);
			std::time_t t = std::time(nullptr);
			std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message OUT: Receiver: IP: " << (*it).getip() << " Port: " << (*it).getport() << " Message: " << msg << std::endl << std::flush;
			sender.close_connection();
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
			std::time_t t = std::time(nullptr);
			std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message OUT: Signal id " << signalid << " send from " << myid << " to " << (*it).getport() << std::endl << std::flush;
			sender.close_connection();
		} else {
			std::cout << "ID: " << myid << " Connection to " << (*it).getport() << " failed." << std::endl << std::flush;
		}
	
	}while(++it != receivers.get_end());
	return -1;
}

// Sends a rumor to all neighbors except the one this node heard it from
int Node::send_all_rumor(Addressbook receivers, int sender_id, int signalid){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		// Don't send it to the one I received it
		if((*it).getid() != sender_id){
			Sender sender((*it).getip(),(*it).getport());
			if((sender.get_connection()) != -1){
				sender.send_signalid(signalid);
				sender.send_id(myid);
				std::time_t t = std::time(nullptr);
				std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message OUT: Signal id " << signalid << " send to " << (*it).getport() << std::endl << std::flush;
				sender.close_connection();
			} else {
				std::cout << "ID: " << myid << " Connection to " << (*it).getport() << " failed." << std::endl << std::flush;
			}
		}
	
	}while(++it != receivers.get_end());
	return -1;
}

// Based on the Addressfile and the graphfile,
// create the Addressbook only with known neighbors
std::list<int> Node::get_nb_ids(std::string gfname, int own_id){
	std::ifstream gifile;
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
	std::cout << "ID: " << myid << "IDs neighboring me:";
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
int Node::sc_exit_node(std::time_t& t, bool& listen_more){
	std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Exit me." << std::endl << std::flush;
	// TODO increase the vectortime
	listen_more = false;
	return -1;
}

// Case EXIT_ALL
int Node::sc_exit_all(std::time_t& t, bool& listen_more){
	std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Exit all." << std::endl << std::flush;
	send_all_signal(book,EXIT_NODE);
	listen_more = false;
	return -1;
}

// Case RECV_MSG
int Node::sc_recv_msg(std::time_t& t, int& confd){
	std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Receive message, buffer size is " << MSG_BUFFER_SIZE << " characters...";
	char a[MSG_BUFFER_SIZE];
	memset(&a[0],0,sizeof(a));
	read(confd,&a,sizeof(a));
	std::cout << "message received." << std::endl << "Content: " << a << std::endl << std::flush;
	return -1;
}

// Case SOCIALISE
int Node::sc_socialise(){
	std::stringstream ss;
	ss << "My ID is: " << myself.getid();
	// increase the vectortime
	vtime[myid - 1] = vtime[myid -1] + 1;

	send_all_msg(neighbors, ss.str());

	// TODO Remove debug cout
	std::cout << "time increased" << std::endl;
	//for (int i = 0; i < book.entrycount(); i++){
	//	vtime[i] = max(vtime[i], vtimestamp[i]);
	//}
	return -1;
}

// Case RUMOR
int Node::sc_rumor(std::time_t& t, int& confd){
	int sender_id = -1;
	read(confd,&sender_id,sizeof(sender_id));
	rumor_counter++;
	if(!heard_rumor){
		std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: A new Rumor." << std::endl << std::flush;
		heard_rumor = true;
		send_all_rumor(neighbors, sender_id, RUMOR);
	}
	if(rumor_counter >= believe_border && !believe_rumor){
		believe_rumor = true;
		Sender sender("localhost",WATCHER_PORT);
		sender.get_connection();
		std::stringstream ss;
		ss << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message OUT: Node " << myid << " believes the rumor." << std::endl;
		std::cout << ss.str();
		sender.send_msg(vtime, ss.str());
		sender.close_connection();
	}
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

		// Pepare the time struct
		std::time_t t = std::time(nullptr);

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

			// exit single node
			case EXIT_NODE : {
						 sc_exit_node(t,listen_more);
						 break;
					 }
					 // exit all nodes
			case EXIT_ALL : {
						sc_exit_all(t,listen_more);
						break;
					}
					// recv msgs with max length of 256 chars
					// TODO check on length
			case RECV_MSG : {
						sc_recv_msg(t, confd);
						break;
					}
					// send a string msg to all my neighbors with my id
			case SOCIALISE : {
						 sc_socialise();
						 break;
					 }
					 // start spreading a rumor
			case RUMOR : {
					     sc_rumor(t, confd);
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
