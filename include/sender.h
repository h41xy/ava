// Prepares a connection based on given ip and port
// Connects to it and messages can then be send until closed
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

#include "constants.h"

class Sender{
	//----getaddrinfo----
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	//----socket()----
	int sockfd;
	//----send()-----
	std::string msg;

	void prepare_connection(); // Prepares the needed structs

	int send_int(const int&); // Writes an interger value to the socket

 public:
	Sender(std::string,int); // ip as string and port as int
	int get_connection();
	int send_msg(std::string);
	int send_msg(std::vector<int>&, std::string);
	int send_vtimestamp(std::vector<int>&);

	// These two basically call send_int(int&)
	int send_signalid(const int&);
	int send_id(const int&);

	int close_connection();
};
