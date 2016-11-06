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
		ip = line.substr(pos_white+1,(line.size()-(pos_colo+pos_white)));
		port = std::stoi(line.substr(pos_colo+1,((line.size()-pos_colo)-1)));

		this->add(Entry(id,ip,port));
	}
	ifile.close();
}

// Fill the Addressbook from a file
void Addressbook::add(Entry nentry){
	book.push_back(nentry);
}

int Addressbook::remove(int id){
	std::list<Entry>::iterator it = book.begin();
	while((*it).getid() != id){
		it++;
		if(it == book.end())
			return -1;
	}
	book.erase(it);
	return 0;
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

std::tuple<Entry,Entry,Entry> Addressbook::return_three_random_entries(int id){
	// random seed...kinda ; )
	std::srand(std::time(0));
	
	// get three random ints
	// the (+ 1) and (+3) are if first is zero
	int first, second, third;
	int size = book.size();
	first = std::rand() % size;
	second = (first  + first + 1) % size;
	third = ((first +3) * 3) % size;

	// get the entries by order in the list
	// like: 0 plus first random value, 0 + second random value, etc....
	Entry efirst, esecond, ethird;

	// first random entry
	std::list<Entry>::iterator it = book.begin();
	std::advance(it, first);
	efirst = *it;

	// second random entry
	it = book.begin();
	std::advance(it, second);
	esecond = *it;

	// third random entry
	it = book.begin();
	std::advance(it,third);
	ethird = *it;

	return std::make_tuple (efirst,esecond,ethird);
}
