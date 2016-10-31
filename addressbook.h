// The Addressbook which contains the IDs, IPs and Ports

#include <string>
#include "entry.h"

class Addressbook{
	// a storage method for entries
 public:
	Addressbook(std::string);
	int add(Entry);
	Entry getbyid(int);
};
