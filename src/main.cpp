// The main file for the ava app
#include <iostream>
#include <string>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>

#include "addressbook.h"
#include "listener.h"
#include "sender.h"

void interact(int acc_confd){
	char buffer[256];
	std::string str;
	std::string quit = "quit";
	do{
		memset(buffer,0,sizeof buffer);
		recv(acc_confd, buffer, 256, 0);
		str = std::string(buffer);
		std::cout << str;
	}while(str.find(quit) == std::string::npos);
	close(acc_confd);

}

/* The run method actually starts the different steps to set up the node
 * - Read one argument as a id
 * - Read a textfile which contains an addressbook, <id, whitespace,IP, colon,port>
 * - Open a port and listening on it. Port depends on ID in addressbook. If msgs arrive, they will be printed to stdout with timestamp.
 * - Choose three random neigbors from the addressbook
 * - Send the own ID once to these three.
 * - Put all send msgs also on stdout with timestamp
 */
int run(char *id_cstr){
	std::string id_str(id_cstr);
	int id = std::stoi(id_str);
	// Read File
	// ID IP:Port
	Addressbook book("doc/example.txt");
	std::cout << "Addressbuch eingelesen.\n";

	// argv ID, lookup ID and listen on the port
	//
	Entry myself = book.getbyid(id);
	std::string myip = myself.getip();
	int myport = myself.getport();
	// remove "my" entry so it doent get chosen as neighbor
	book.remove(id);
	std::cout << "My port is: " << myport << "\n";

	// choose three random other IDs
	//
	std::tuple<Entry,Entry,Entry> randoms = book.return_three_random_entries(id);
	int port_nb_one, port_nb_two, port_nb_three;
	port_nb_one = std::get<0>(randoms).getport();
	port_nb_two = std::get<1>(randoms).getport();
	port_nb_three = std::get<2>(randoms).getport();
	std::cout << "My neighbors ports are: " << port_nb_one << " " << port_nb_two << " " << port_nb_three << "\n";

	// send ID to neighbours
	//
	// send msg with timestamp
	//Sender sender("localhost",25002);
	//sender.get_connection();
	//sender.send_msg("hi, im norbert\n");
	//sender.close_connection();

	// listen on the port
	//
	int confd;
	char buffer[256];
	std::string msg, time, quit, exit;
	quit = "quit";
	exit = "exit";

	Listener listener(myport);
	listener.prepare_and_listen();
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
			std::cout << std::put_time(std::localtime(&t), "Time > %H:%M:%S ") << "Message: " << msg;
		}while(msg.find(quit) == std::string::npos && msg.find(exit) == std::string::npos);
		close(confd);

	}while(msg.find(exit) == std::string::npos);
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
