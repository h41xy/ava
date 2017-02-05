// The Addressbook which contains the IDs, IPs and Ports
// Those include id, ip and port.
// All get read from a given filepath and parsed into entries.
// They get stored as a linked list.

#include "addressbook.h"

Addressbook::Addressbook() : Addressbook(ADDRESSFILE) {};

// Read from given filename
Addressbook::Addressbook(std::string fname){
	fillfromfile(fname);
}

// Parser for translating the contains of the file to Entries.
// Opens the file and looks for entries.
// The entries have to be <id,whitespace,ip,colon,port>
void Addressbook::fillfromfile(std::string fname){

	std::ifstream ifile;
	ifile.open(fname);

	std::string line, ip;
	int id, port;

	// For every line
	// find the seperating chars <whitespace, colon>
	// build and store substrings based on the seperators
	while( getline(ifile,line)){
		std::size_t pos_white = line.find(" ");
		std::size_t pos_colo = line.find(":");

		id = std::stoi(line.substr(0, pos_white));
		//ip = line.substr(pos_white+1,(line.size()-(pos_colo+pos_white)));
		ip = line.substr(pos_white+1,((line.size()-(line.size()-pos_colo)-pos_white)-1));
		port = std::stoi(line.substr(pos_colo+1,((line.size()-pos_colo)-1)));

		this->add(Entry(id,ip,port));
	}
	ifile.close();
}

void Addressbook::add(Entry nentry){
	book.push_back(nentry);
}

// Returns a Entry identified by id
// can be combined with remove when good return is found
// TODO work with list.end()
Entry Addressbook::getbyid(int id){
	// Iterates over the list until the last entry is reached
	std::list<Entry>::iterator it = book.begin();
	while((*it).getid() != id){
		it++;
		if(it == book.end())
			return Entry(-1,"",-1); // Return NULL like java not possible, altenative not found yet.
	}
	return (*it);
}

unsigned int Addressbook::size(){
	return book.size();
}

std::list<Entry>::iterator Addressbook::begin(){
	std::list<Entry>::iterator it = book.begin();
	return it;
}

std::list<Entry>::iterator Addressbook::end(){
	return book.end();
}
