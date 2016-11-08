// The addressbook contains Entries about neighbors.
// Those include id, ip and port.
// All get read from a given filepath and parsed into entries.
// They get stored as a linked list.

#include <string>
#include <fstream>
#include <list>
#include <tuple>
#include <ctime> // included for random seed

#include "entry.h"

class Addressbook{
	std::list<Entry> book;
 public:
	Addressbook(std::string);
	void fillfromfile(std::string);
	void add(Entry);
	int remove(int); // Removes an Entry identified by id, return -1 if not found
	Entry getbyid(int); // Returns found Entry, if not found returns an Entry with (-1,"",-1). pretty ulgy
	int entrycount();
	std::tuple<Entry,Entry,Entry> return_three_random_entries(); //Returns three random entries
};
