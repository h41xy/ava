// An Entry in the Addressbook
// Contains the id and port as int, the ip as string

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

