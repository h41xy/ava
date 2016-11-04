// A listener class which gets a port.
// It opens a socket on this port and listens for inc msgs.
// Those get send back with timestamps
#include <tuple>

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
	int prepare_and_listen();
	int accept_connection();
};
