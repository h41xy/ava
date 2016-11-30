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
}

// Sends a string to all addresses in the given book
int Node::send_all_msg(Addressbook receivers, std::string msg){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_msg(msg);
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
	int confd = -1;
	bool listen_more = true;
	// The rumor counter has to be initialized, unexpected behaviour is the result otherwise
	rumor_counter = 0;
	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		int msg_id = -1;
		std::time_t t = std::time(nullptr);
		read(confd,&msg_id,sizeof(msg_id));
		switch(msg_id){

			// exit single node
			case EXIT_NODE : {
						 std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Exit me." << std::endl << std::flush;
						 listen_more = false;
						 break;
					 }
			 // exit all nodes
			case EXIT_ALL : {
						std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Exit all." << std::endl << std::flush;
						send_all_signal(book,EXIT_NODE);
						listen_more = false;
						break;
					}
			// recv msgs with max length of 256 chars
			// TODO check on length
			case RECV_MSG : {
						std::cout << "ID: " << myid << std::put_time(std::localtime(&t), " Time > %H:%M:%S ") << "Message IN: Receive message, buffer size is " << MSG_BUFFER_SIZE << " characters...";
						char a[MSG_BUFFER_SIZE];
						memset(&a[0],0,sizeof(a));
						read(confd,&a,sizeof(a));
						std::cout << "message received." << std::endl << "Content: " << a << std::endl << std::flush;
						break;
					}
			// send a string msg to all my neighbors with my id
			case SOCIALISE : {
						 std::stringstream ss;
						 ss << "My ID is: " << myself.getid();
						 send_all_msg(neighbors, ss.str());
						 break;
					 }
			 // start spreading a rumor
			// TODO own function
			case RUMOR : {
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
						     sender.send_msg(ss.str());
						     sender.close_connection();
					     }
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
