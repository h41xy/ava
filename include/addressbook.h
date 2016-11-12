// The addressbook contains Entries about neighbors.
// Those include id, ip and port.
// All get read from a given filepath and parsed into entries.
// They get stored as a linked list.

#include <string>
#include <fstream>
#include <list>
#include <tuple>
#include <algorithm>
#include <ctime> // included for random seed

#include "entry.h"

class Addressbook{
	std::list<Entry> book;
 public:
	Addressbook();
	Addressbook(std::string);
	Addressbook(std::string, std::list<int>);
	void fillfromfile(std::string);
	void fillfromfile_nbs_only(std::string,std::list<int>);
	void add(Entry);
	int remove(int); // Removes an Entry identified by id, return -1 if not found
	Entry getbyid(int); // Returns found Entry, if not found returns an Entry with (-1,"",-1). pretty ulgy
	int entrycount();
	std::list<Entry>::iterator get_iterator();
	Entry get_entry_at(std::list<Entry>::iterator);
	std::list<Entry>::iterator get_end();
	std::tuple<Entry,Entry,Entry> return_three_random_entries(); //Returns three random entries
};
