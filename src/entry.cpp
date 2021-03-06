// An Entry in the Addressbook
// Contains the id and port as int, the ip as string

#include "entry.h"

Entry::Entry() : Entry(-1,"localhost",-1) {}

Entry::Entry (int id, std::string ip, int port)
	: id(id)
	, port(port)
{
	std::strcpy(this->ip, ip.c_str());
}


int Entry::getid() const{
	return this->id;
}
std::string Entry::getip() const{
	return this->ip;
}
int Entry::getport() const{
	return this->port;
}
