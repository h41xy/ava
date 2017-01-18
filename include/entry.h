// An Entry in the Addressbook
// Contains the id and port as int, the ip as string

#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <cstring>

#include "constants.h"

class Entry{
	int id, port;
	char ip[IP_BUFFER_SIZE]; // length of max ip 123.123.123.123
 public:
	Entry (int,std::string,int);
	Entry ();
	int getid() const;
	std::string getip() const;
	int getport() const;
};
#endif // ENTRY_H
