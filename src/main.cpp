// The main file for the ava app
#include <iostream>
#include <string>
#include <tuple>

#include "addressbook.h"

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
	// output msgs with timestamp
	//
	// send ID to neighbours
	//
	// send msg with timestamp
	return -1;
}

int main ( int argc, char *argv[]) {
	if(argc > 1)
		run(argv[1]);
	else
		std::cout << "Please give a ID as Argument.\n";
}
