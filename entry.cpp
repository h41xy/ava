// An Entry in the Addressbook

#include "entry.h"

Entry::Entry (int id, std::string ip, int port){
	this->id = id;
	this->ip = ip;
	this->port = port;
}
