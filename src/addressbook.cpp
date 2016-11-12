// The Addressbook which contains the IDs, IPs and Ports
// Those include id, ip and port.
// All get read from a given filepath and parsed into entries.
// They get stored as a linked list.

#include "addressbook.h"

Addressbook::Addressbook(){};

// Read from given filename
Addressbook::Addressbook(std::string fname){
	fillfromfile(fname);
}

Addressbook::Addressbook(std::string fname, std::list<int> ids_neighboring_me){
	fillfromfile_nbs_only(fname, ids_neighboring_me);
}

// Parser for translating the contains of the file to Entries.
// Opens the file and looks for entries.
// The entries have to be <id,whitespace,ip,colon,port>
// Only adds entries which ID is found in the given list
void Addressbook::fillfromfile_nbs_only(std::string fname, std::list<int> ids_neighboring_me){

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
		// Add only if the id is neighboring me
		if ( std::find(ids_neighboring_me.begin(),ids_neighboring_me.end(),id) != ids_neighboring_me.end()){
			ip = line.substr(pos_white+1,((line.size()-(line.size()-pos_colo)-pos_white)-1));
			port = std::stoi(line.substr(pos_colo+1,((line.size()-pos_colo)-1)));

			this->add(Entry(id,ip,port));
		}
	}
	ifile.close();
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

// Removes an Entry from the addressbook identified by his id
// returns -1 if not found.
int Addressbook::remove(int id){
	// Iterates over the list until the last entry is reached
	std::list<Entry>::iterator it = book.begin();
	while((*it).getid() != id){
		if(it == book.end())
			return -1;
		it++; // put it in while( (++(*it).getid()) != id) ?
	}
	book.erase(it);
	return 0;
}

// Returns a Entry identified by id
// can be combined with remove when good return is found
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

int Addressbook::entrycount(){
	return book.size();
}

std::list<Entry>::iterator Addressbook::get_iterator(){
	std::list<Entry>::iterator it = book.begin();
	return it;
}

std::list<Entry>::iterator Addressbook::get_end(){
	return book.end();
}

Entry Addressbook::get_entry_at(std::list<Entry>::iterator it){
	return (*it);
}

// Returns three random entries fomr the current addressbook as a tuple
std::tuple<Entry,Entry,Entry> Addressbook::return_three_random_entries(){
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
