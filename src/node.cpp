// The node
#include "node.h"

Node::Node(char* id_cstr){
	std::string id_str(id_cstr);
	myid = std::stoi(id_str);
	//----Read File
	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	book = Addressbook("doc/example.txt", get_nb_ids("doc/example_graph.txt", myid));
}

// Sends a string to all neighbors in the addressbook
int Node::send_all_msg(std::string msg){
	std::list<Entry>::iterator it = book.get_iterator();
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_msg(msg);
			std::time_t t = std::time(nullptr);
			std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message OUT: " << msg;
			sender.close_connection();
			return 0;
		}
	
	}while(++it != book.get_end());
	return -1;
}

// Based on the Addressfile and the graphfile,
// create the Addressbook only with known neighbors
std::list<int> Node::get_nb_ids(std::string gfname, int own_id){
	std::ifstream gifile;
	gifile.open(gfname);

	std::string line;
	std::list<int> ids_neighboring_me;
	ids_neighboring_me.push_back(own_id); // I neighbor myself

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
	std::cout << "IDs neighboring me:";
	for(auto v : ids_neighboring_me)
		std::cout << " " << v;
	std::cout << std::endl;
	return ids_neighboring_me;
}

int Node::send_all_signal(int signalid){
	std::list<Entry>::iterator it = book.get_iterator();
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_signalid(signalid);
			std::time_t t = std::time(nullptr);
			std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message OUT: Exit all neighbors.";
			sender.close_connection();
			return 0;
		}
	
	}while(++it != book.get_end());
	return -1;
}
/* The run method actually starts the different steps to set up the node
 * - Read one argument as a id
 * - Read a textfile which contains an addressbook, <id, whitespace,IP, colon,port>
 * - Open a port and listening on it. Port depends on ID in addressbook. If msgs arrive, they will be printed to stdout with timestamp.
 * - Choose three random neigbors from the addressbook
 * - Send the own ID once to these three.
 * - Put all send msgs also on stdout with timestamp
 */
int Node::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	// remove "my" entry so it doesnt get chosen as neighbor
	book.remove(myid);
	std::cout << "My port is: " << myself.getport() << "\n";


	// listen on the port
	int confd;
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop
	bool listen_more = true;
	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		int msg_id = -1;
		std::time_t t = std::time(nullptr);
		read(confd,&msg_id,sizeof(msg_id));
		switch(msg_id){

			case EXIT_NODE : {
				std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message IN: Exit me and all neighbors.";
				send_all_signal(EXIT_NODE);
				listen_more = false;
				break;
				}
			case RECV_MSG : {
				std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message IN: Receive message, buffer size is " << MSG_BUFFER_SIZE << " characters...";
				char a[MSG_BUFFER_SIZE];
				memset(&a[0],0,sizeof(a));
				read(confd,&a,sizeof(a));
				std::cout << "message received." << std::endl << "Content: " << a << std::endl;
				break;
				}
			case SOCIALISE : {
				std::stringstream ss;
				ss << "My ID is: " << myself.getid();
				send_all_msg(ss.str());
				break;
				}
			default :
				std::cout << "I don't know this signal id. Close connection.\n";
				break;
		}
		close(confd);

	}while(listen_more);
	std::cout << "Node exited." << std::endl;
	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}

