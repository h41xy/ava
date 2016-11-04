// The main file for the ava app
#include <iostream>
#include <string>
#include <tuple>

#include "addressbook.h"
#include "listener.h"

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
	int myport = book.getbyid(id).getport();
	// remove "my" entry so it doent get chosen as neighbor
	book.remove(id);
	std::cout << "My port is: " << myport << "\n";

	// choose three other IDs
	//
	std::tuple<Entry,Entry,Entry> randoms = book.return_three_random_entries(id);
	int port_nb_one, port_nb_two, port_nb_three;
	port_nb_one = std::get<0>(randoms).getport();
	port_nb_two = std::get<1>(randoms).getport();
	port_nb_three = std::get<2>(randoms).getport();
	std::cout << "My neighbors ports are: " << port_nb_one << " " << port_nb_two << " " << port_nb_three << "\n";
	Listener("localhost",25001);
	// output msgs with timestamp
	//
	// send ID to neighbours
	//
	// send msg with timestamp
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
