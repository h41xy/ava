// The Addressbook which contains the IDs, IPs and Ports

#include "addressbook.h"
#include <iostream>

Addressbook::Addressbook(std::string fname){
	// open file
	std::ifstream ifile;
	ifile.open(fname);
	std::string line;
	while( getline(ifile,line)){
		std::cout << line << '\n';
		std::size_t pos_white = line.find(" ");
		std::size_t pos_colo = line.find(":");
		std::cout << pos_white << " " << pos_colo << "\n";
		std::string id_str = line.substr(0, pos_white);
		int id = std::stoi(id_str);
		std::string ip = line.substr(pos_white+1,((line.size()-(line.size()-pos_colo)-pos_white)-1));
		std::string port_str = line.substr(pos_colo+1,((line.size()-pos_colo)-1));
		int port = std::stoi(pot_str);
		std::cout << id << " " << ip << ":" << port << "\n";
	}
	ifile.close();
	// while != eof
	// create entry from line
	// add to addressbook
	// close
}

// Fill the Addressbook from a file
int Addressbook::add(Entry nentry){
	return -1;
}

Entry Addressbook::getbyid(int id){
	return Entry(1,"test",2);
}
