// sends given messages to given ip,ports
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

	void prepare_connection();

 public:
	Sender(std::string,int); // ip as string and port as int
	int get_socket();
	int get_connection();
	int send_msg(std::string);
	int close_connection();
	void connect_a();
};
