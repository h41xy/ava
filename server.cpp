/*
 * =====================================================================================
 *
 *       Filename:  server.cpp
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

	int sockfd, confd; // listen on socketfd, new connection on confd

	// int socket(int domain, int type, int protocol);
	// domain is AF_INET
	// type is SOCK_STREAM
	// protocol is 0
	//sockfd = socket(AF_INET,SOCK_STREAM,0);
	// instead use the values direct from getaddrinfo
	sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	cout << "socket created\n";

	//--------------------------------------------------------------------------------

	//---------------------------------bind-------------------------------------------

	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	// sockfd is sockfd
	// addr is the address to be 
	// addrlen is the length (sizeof)
	bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen);
	//--------------------------------------------------------------------------------

	//---------------------------------listen-----------------------------------------

	// int listen(int sockfd, int backlog);
	// sockfd is sockfd
	// backlog is defines the maximum length to which the queue of pending connections for sockfd may grow.
	listen(sockfd,5);
	cout << "Listening\n";
	//--------------------------------------------------------------------------------

	//---------------------------------accept-----------------------------------------

	// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	// addr points to a struct which gets filled with the peer address
	// addrlen  soze of the struct (sizeof)
	struct sockaddr_storage peer_addr;
	// sockaddr_storage is a generic struct for all domain specific types
	// read on in man 7 socket
	socklen_t addr_size;

	//char *buffer[256];
	char buffer[256];
	string quit = "quit";
	string exit = "exit";
	string str;

	addr_size = sizeof peer_addr;
	do{
		confd = accept(sockfd, (struct sockaddr *)&peer_addr, &addr_size);
		cout << "Someone connected : D";
		//send(confd, "What a time to be alive!", strlen("What a time to be alive!"), 0);

		do{
			memset(buffer,0,sizeof buffer);
			recv(confd, buffer, 256, 0);
			str = string(buffer);
			cout << str;
		}while(str.find(quit) == std::string::npos);
		close(confd);
	}while(str.find(exit) == std::string::npos);
	close(sockfd);
	//--------------------------------------------------------------------------------
	//---------------------------------close or shutdown------------------------------
	//--------------------------------------------------------------------------------
}
