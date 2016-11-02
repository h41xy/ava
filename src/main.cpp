// The main file for the ava app
#include <iostream>
#include <string>

#include "addressbook.h"

int run(char *id_cstr){
	std::string id_str(id_cstr);
	int id = std::stoi(id_str);
	// Read File
	// ID IP:Port
	Addressbook book("doc/example.txt");

	// argv ID, lookup ID and listen on the port
	int myport = book.getbyid(id).getport();
	std::cout << myport << "\n";
	//
	// choose three other IDs
	//
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
