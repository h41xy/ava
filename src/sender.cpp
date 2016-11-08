#include "sender.h"

Sender::Sender(std::string ip, int port){
	prepare_connection();
	// getaddrinfo dont like strings...
	// conversion to cstrings
	const char *port_cstr = std::to_string(port).c_str();
	getaddrinfo("127.0.0.1","25002", &hints, &serverinfo);
}

void Sender::prepare_connection(){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM;
}

int Sender::get_connection(){
	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	if(sockfd == -1)
		return -1;
	int result = connect(sockfd,serverinfo->ai_addr,serverinfo->ai_addrlen);
	if (result == -1)
		return -1;
	return 0;
}

int Sender::send_msg(std::string msg){
	send(sockfd,msg.c_str(),msg.size(),0);
	return -1;
}

int Sender::close_connection(){
	close(sockfd);
	return -1;
}
