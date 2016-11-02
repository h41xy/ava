// The Addressbook which contains the IDs, IPs and Ports

#include "addressbook.h"

Addressbook::Addressbook(std::string fname){
	fillfromfile(fname);
}

int Addressbook::fillfromfile(std::string fname){
	// open file
	// while != eof
	// create entry from line
	// add to addressbook
	// close
	std::ifstream ifile;
	ifile.open(fname);

	std::string line, ip;
	int id, port;
	while( getline(ifile,line)){
		std::size_t pos_white = line.find(" ");
		std::size_t pos_colo = line.find(":");

		id = std::stoi(line.substr(0, pos_white));
		ip = line.substr(pos_white+1,((line.size()-(line.size()-pos_colo)-pos_white)-1));
		port = std::stoi(line.substr(pos_colo+1,((line.size()-pos_colo)-1)));

		//Entry nentry(id,ip,port);
		this->add(Entry(id,ip,port));
	}
	ifile.close();
	return -1;
}

// Fill the Addressbook from a file
int Addressbook::add(Entry nentry){
	if (index < MAXNODES){
		book[index++] = nentry;
		return 0;
	}
	// Throw Error
	return -1;
}

Entry Addressbook::getbyid(int id){
	return Entry(1,"test",2);
}

int Addressbook::entrycount(){
	return index;
}
