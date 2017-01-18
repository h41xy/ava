#include "sender.h"

// Prepares a connection with given parameters
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

// TODO check if connected
int Sender::send_message(Message message){
	write(sockfd,&message,sizeof(Message));
	return -1;
}

// Sends the vtimestamp
// TODO check if connected
// TODO write own timestamp class
int Sender::send_vtimestamp(std::vector<int>& vtimestamp){
	// write every value sequentially to the socket
	// the read on the other side will pick them up the same way and write
	// them in a new vector
	for(unsigned int i = 0; i < vtimestamp.size(); i++){
		write(sockfd,&vtimestamp[i],sizeof(vtimestamp[i]));
	}
	return -1;
}

// TODO check if connected
int Sender::close_connection(){
	close(sockfd);
	return -1;
}
