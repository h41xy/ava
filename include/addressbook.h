// The addressbook contains Entries about neighbors.
// Those include id, ip and port.
// All get read from a given filepath and parsed into entries.
// They get stored as a linked list.

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <string>
#include <fstream>
#include <list>
#include <tuple>
#include <algorithm>
#include <ctime> // included for random seed

#include "entry.h"

class Addressbook{
public:
	Addressbook();
	Addressbook(std::string);

	void fillfromfile(std::string);
	void add(Entry);
	// TODO this method needs rework
	Entry getbyid(int); // Returns found Entry, if not found returns an Entry with (-1,"",-1). pretty ulgy
	int size();
	std::list<Entry>::iterator begin();
	std::list<Entry>::iterator end();
private:
	std::list<Entry> book;
};

#endif // ADDRESSBOOK_H
