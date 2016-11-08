// An Entry in the Addressbook
// Contains the id and port as int, the ip as string

#include "entry.h"

Entry::Entry (int id, std::string ip, int port){
	// maybe change to linked list so next can be used
	this->id = id;
	this->ip = ip;
	this->port = port;
}

Entry::Entry(){}

int Entry::getid(){
	return this->id;
}
std::string Entry::getip(){
	return this->ip;
}
int Entry::getport(){
	return this->port;
}
