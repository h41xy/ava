// The Addressbook which contains the IDs, IPs and Ports

#include "addressbook.h"

Addressbook::Addressbook(std::string fname){
	fillfromfile(fname);
}

void Addressbook::fillfromfile(std::string fname){
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

		this->add(Entry(id,ip,port));
	}
	ifile.close();
}

// Fill the Addressbook from a file
void Addressbook::add(Entry nentry){
	book.push_back(nentry);
}

Entry Addressbook::getbyid(int id){
	std::list<Entry>::iterator it = book.begin();
	while((*it).getid() != id){
		it++;
		if(it == book.end())
			return Entry(-1,"",-1);
	}
	return (*it);
}

int Addressbook::entrycount(){
	return book.size();
}
