// The Addressbook which contains the IDs, IPs and Ports

#include <string>
#include <fstream>
#include "entry.h"

class Addressbook{
	// a storage method for entries
	const int MAXNODES = 100;
	Entry book[MAXNODES];
	int index = 0;
 public:
	Addressbook(std::string);
	int add(Entry);
	Entry getbyid(int);
};
