#include <iostream>
// Most i/o, typicaly everywhere included
#include <sys/types.h>
//definitions of datatypes used in system calls
#include <sys/socket.h>
// socket structure definitions
//#include <netinet/in.h>
//// constants and sturctures for internet domain addresses
#include <netdb.h>
#include <cstring>
// for memset
#include <unistd.h>
// for close
#include <string>
using namespace std;

int main(int argc, char *argv[]){

	// Order is:
	// getaddrinfo()
	// socket()
	// bind()
	// listen()
	// accept()
	// close() or shutdown()

	//-------------------------getaddrinfo--------------------------------------------

	// int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	// inode is the serveraddress, www.example.org or IP
	// service is "http" or portnr
	// hints is a addinfo struct which sets parameters like "use IPv4" "its a tcp stream socket" and so on...
	// res is a pointer to a pointer which will point to the results...get it?

	struct addrinfo hints; // This contains infos for selecting the socket address structures in serverinfo
	struct addrinfo *serverInfo; // This contains the Serverinfo later for binding to the socket

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE; // assign localhost

	getaddrinfo("127.0.0.1", "25002", &hints, &serverInfo);

	//--------------------------------------------------------------------------------

	//-------------------------socket-------------------------------------------------

	int sockfd;//, confd; // listen on socketfd, new connection on confd

	// int socket(int domain, int type, int protocol);
	// domain is AF_INET
	// type is SOCK_STREAM
	// protocol is 0
	//sockfd = socket(AF_INET,SOCK_STREAM,0);
	// instead use the values direct from getaddrinfo
	if((sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1)
		cout << "socket failed\n";

	//--------------------------------------------------------------------------------

	//---------------------------------connect----------------------------------------
	if(connect(sockfd,serverInfo->ai_addr,serverInfo->ai_addrlen) == -1)
		cout << "SHIEEEEET";
	cout << "connecting worked\n";
	string msg = "YES MUTHAFUCKA!\n";
	//int len = msg.size();
	if(send(sockfd,"Anus time. quit\n",17,0) == -1)
		cout << "send failed";
	cout << "send succeded";
	close(sockfd);

}
