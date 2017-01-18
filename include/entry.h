// An Entry in the Addressbook
// Contains the id and port as int, the ip as string

#ifndef ENTRY_H
#define ENTRY_H

#include <string>

class Entry{
	int id, port;
	std::string ip;
 public:
	Entry (int,std::string,int);
	Entry ();
	int getid() const;
	std::string getip() const;
	int getport() const;
};
#endif // ENTRY_H
