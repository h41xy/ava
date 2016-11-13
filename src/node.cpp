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

// Sends the given id to three neighbor ports
// Creates a Sender object which creates and connects
// a socket on the localhost and a port.
// Sends the message and closes the socket.
int Node::send_id_to_neighbor(std::string recv_ip, int recv_port){
	// send msg with timestamp
	std::ostringstream os;
	os << "Hi, my ID is: " << myself.getid() << " quit\n";
	std::string msg_id = os.str();

	//Sender sender("localhost",25002); // temp for testing
	Sender sender(recv_ip, recv_port);
	if((sender.get_connection()) != -1){
		sender.send_msg(msg_id);
		std::time_t t = std::time(nullptr);
		std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message OUT: " << msg_id;
		sender.close_connection();
		return 0;
	}
	return -1;
}

// Sends a string to all neighbors in the addressbook
int Node::send_msg_to_all(std::string msg){
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

// Choose three random ids aka ports from the addressbook and send the own id to them
int Node::socialise_myself(){

	// choose three random other IDs from the addressbook and get their ports
	//
	std::tuple<Entry,Entry,Entry> randoms = book.return_three_random_entries();
	Entry nb_one, nb_two, nb_three;
	nb_one = std::get<0>(randoms);
	nb_two = std::get<1>(randoms);
	nb_three = std::get<2>(randoms);
	std::cout << nb_one.getip() << "\n";
	std::cout << "My neighbors ports are: " << nb_one.getport() << " " << nb_two.getport() << " " << nb_three.getport() << "\n";

	// send ID to neighbours
	//
	if(send_id_to_neighbor(nb_one.getip(), nb_one.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
	if(send_id_to_neighbor(nb_two.getip(), nb_two.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
	if(send_id_to_neighbor(nb_three.getip(), nb_three.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
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
		read(confd,&msg_id,sizeof(msg_id));
		switch(msg_id){
			case 1 : 
				std::cout << "Case 1 happend.\n";
				listen_more = false;
				break;
			case 2 :
				std::cout << "Case 2 happend.\n";
				break;
			default :
				std::cout << "I don't know this signal id. Close connection.\n";
				break;
		}
		close(confd);

	}while(listen_more);
	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}

