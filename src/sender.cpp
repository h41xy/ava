#include "sender.h"

Sender::Sender(std::string ip, int port){
	prepare_connection();
	// getaddrinfo dont like strings...
	// conversion to cstrings
	const char *ip_cstr = ip.c_str(); // Wrap in getaddrinfo ?
	const char *port_cstr = std::to_string(port).c_str();
	getaddrinfo(ip_cstr,port_cstr, &hints, &serverinfo);
}

// Prepare the needed structs
void Sender::prepare_connection(){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM;
}

// Creates a socket and connects to ip and port
// return -1 if either socket or connect fails
int Sender::get_connection(){
	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	if(sockfd == -1)
		return -1;
	// I messed up the brackets one time therefore an extra int is created
	int result = connect(sockfd,serverinfo->ai_addr,serverinfo->ai_addrlen);
	if (result == -1)
		return -1;
	return 0;
}

// Sends a messxage on the established connection
// Currently the message must contain the word "quit" and has to end with a \n
int Sender::send_msg(std::string msg){
	send(sockfd,msg.c_str(),msg.size(),0);
	return -1;
}

int Sender::send_signalid(int signalid){
	write(sockfd,&signalid,sizeof(signalid));
	return -1;
}

int Sender::close_connection(){
	close(sockfd);
	return -1;
}
