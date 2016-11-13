// Prepares a connection based on given ip and port
// Connects to it and messages can then be send until closed
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>

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
	int send_msg(std::string); //Message muist contain the word "quit" and end with \n
	int send_signalid(int);
	int close_connection();
};
