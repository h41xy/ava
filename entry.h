// An Entry in the Addressbook

#include <string>

class Entry{
	int id, port;
	std::string ip;
 public:
	Entry (int,std::string,int);
	Entry ();
	int getid();
	std::string getip();
	int getport();
};

