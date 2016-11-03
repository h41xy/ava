// The Addressbook which contains the IDs, IPs and Ports

#include <string>
#include <fstream>
#include <list>
#include <tuple>
#include <ctime>
#include "entry.h"

class Addressbook{
	std::list<Entry> book;
 public:
	Addressbook(std::string);
	void fillfromfile(std::string);
	void add(Entry);
	int remove(int);
	Entry getbyid(int);
	int entrycount();
	std::tuple<Entry,Entry,Entry> return_three_random_entries(int);
};
