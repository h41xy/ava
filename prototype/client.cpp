/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  A progam to play around with sockets
 *
 *        Version:  1.0
 *        Created:  10/28/2016 03:12:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  h41
 *   Organization:  htwsaar
 *
 * =====================================================================================
 */

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
// and write
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
	hints.ai_flags = AI_PASSIVE; // assign localhost

	getaddrinfo(NULL, "25003", &hints, &serverInfo);

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
	cout << "connected\n";

	cout << "connecting worked\n";
	string msg = "YES MUTHAFUCKA!\n";
	//int len = msg.size();
	//if(send(sockfd,"Anus time. quit\n",17,0) == -1)
	//	cout << "send failed";
	//cout << "send succeded";
	//write(sockfd,"Anus time. quit\n",17);
	int send = 2;
	const void* cvp;
	cvp = &send;
	write(sockfd,cvp,sizeof(send));
	close(sockfd);
	//--------------------------------------------------------------------------------
	//---------------------------------bind-------------------------------------------

	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	// sockfd is sockfd
	// addr is the address to be 
	// addrlen is the length (sizeof)
	//bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen);

	////--------------------------------------------------------------------------------

	////---------------------------------listen-----------------------------------------

	//// int listen(int sockfd, int backlog);
	//// sockfd is sockfd
	//// backlog is defines the maximum length to which the queue of pending connections for sockfd may grow.
	//listen(sockfd,5);

	////--------------------------------------------------------------------------------

	////---------------------------------accept-----------------------------------------

	//string quit = "quit";

	//addr_size = sizeof peer_addr;
	//while(1){
	//	confd = accept(sockfd, (struct sockaddr *)&peer_addr, &addr_size);
	//	send(confd, "What a time to be alive!", strlen("What a time to be alive!"), 0);

	//	string str;
	//	do{
	//		memset(buffer,0,sizeof buffer);
	//		recv(confd, buffer, 256, 0);
	//		str = string(buffer);
	//		cout << str;
	//	}while(str.find(quit) == std::string::npos);
	//	close(confd);
	//	break;
	//}
	//--------------------------------------------------------------------------------
	//---------------------------------close or shutdown------------------------------
	//--------------------------------------------------------------------------------
}
