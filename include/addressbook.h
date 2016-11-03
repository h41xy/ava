// The Addressbook which contains the IDs, IPs and Ports

#include <string>
#include <fstream>
#include <list>
#include "entry.h"

class Addressbook{
	std::list<Entry> book;
 public:
	Addressbook(std::string);
	void fillfromfile(std::string);
	void add(Entry);
	Entry getbyid(int);
	int entrycount();
};
