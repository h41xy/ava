// The node
// I will create a header file in the future ; )
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>

#include "addressbook.h"
#include "listener.h"
#include "sender.h"

// Sends the given id to three neighbor ports
// Creates a Sender object which creates and connects
// a socket on the localhost and a port.
// Sends the message and closes the socket.
int send_id_to_neighbor(int myid, std::string recv_ip, int recv_port){
	// send msg with timestamp
	std::ostringstream os;
	os << "Hi, my ID is: " << myid << " quit\n";
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
int send_msg_to_all(Addressbook book, std::string msg){
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
int socialise_myself(Addressbook book, Entry myself){

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
	if(send_id_to_neighbor(myself.getid(), nb_one.getip(), nb_one.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
	if(send_id_to_neighbor(myself.getid(), nb_two.getip(), nb_two.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
	if(send_id_to_neighbor(myself.getid(), nb_three.getip(), nb_three.getport()) == -1)
		std::cout << "A connect to neighbor failed.\n";
	return -1;
}

std::list<int> get_nb_ids(std::string gfname, int own_id){
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
// this should be the order, currently he first sends messages and then listens
int run(char *id_cstr){
	std::string id_str(id_cstr);
	int id = std::stoi(id_str);
	//----Read File
	// Create a addressbook based on the neighboring IDs found in 
	// doc/example_graph.txt and the addresses found in doc/example.txt
	Addressbook book("doc/example.txt", get_nb_ids("doc/example_graph.txt", id));
	std::cout << "Addressbuch eingelesen.\n";
	//----

	// Lookup the id from argv and get my associated port
	//

	Entry myself = book.getbyid(id);
	std::string myip = myself.getip();
	int myport = myself.getport();
	// remove "my" entry so it doesnt get chosen as neighbor
	book.remove(id);
	std::cout << "My port is: " << myport << "\n";


	// listen on the port
	//
	int confd;
	char buffer[256];
	std::string msg, time, quit, exit, socialise;
	quit = "quit";
	exit = "exit";
	socialise = "socialise";

	Listener listener(myport);
	listener.create_and_listen();
	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		do{
			memset(buffer,0,sizeof buffer);
			if((recv(confd, buffer, 256, 0)) == -1)
				break;
			msg = std::string(buffer);

			// get time
			std::time_t t = std::time(nullptr);
			// output msgs with timestamp
			//
			std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message IN: " << msg;

			//Solicite with neighbors
			if ( msg.find(socialise) != std::string::npos ){
				std::cout << "Socialising...\n";
				socialise_myself(book, myself); // when header file exists, book and myself will be global
			}

		}while(msg.find(quit) == std::string::npos && msg.find(exit) == std::string::npos);
		close(confd);

	}while(msg.find(exit) == std::string::npos);
	exit.append("\n");
	send_msg_to_all(book, exit);
	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}
/* The main method to start a single node.
 * It requires one argument which is used as the own ID
 */
int main ( int argc, char *argv[]) {
	if(argc > 1)
		run(argv[1]);
	else
		std::cout << "Please give a ID as Argument.\n";
}
