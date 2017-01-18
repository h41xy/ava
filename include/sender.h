// Prepares a connection based on given ip and port
// Connects to it and messages can then be send until closed
#ifndef SENDER_H
#define SENDER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

#include "constants.h"
#include "entry.h"
#include "message.h"

class Sender{
	//----getaddrinfo----
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	//----socket()----
	int sockfd;
	//----send()-----
	std::string msg;

	void prepare_connection(); // Prepares the needed structs

 public:
	Sender(std::string,int); // ip as string and port as int
	int get_connection();

	int send_message(Message);
	int send_vtimestamp(std::vector<int>&);

	int close_connection();
};
#endif // SENDER_H
