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
public:
	Sender();
	Sender(std::string,int); // ip as string and port as int
	int get_connection();

	int send_message(Message);

	int close_connection();

private:
	//----getaddrinfo----
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	//----socket()----
	int sockfd;
	//----send()-----
	std::string msg;

	void prepare_connection(); // Prepares the needed structs
};
#endif // SENDER_H
