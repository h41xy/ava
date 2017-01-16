// A listener class which gets a port.
// It opens a socket on this port and listens for incoming connections

#ifndef LISTENER_H
#define LISTENER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <string>
#include <cstring>
#include <iostream>

class Listener{
	//-----getaddrinfo()----
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	//----socket()----
	int sockfd;
	//----accept()----
	int confd;
	struct sockaddr_storage peer_addr;
	socklen_t addr_size;

 public:
	Listener(int); //port
	int create_and_listen(); // Creates the socket, binds it to local and starts listening
	int accept_connection();
	int close_socket();
};
#endif // LISTENER_H
