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

int Sender::get_socket(){
	return (sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol));
}

int Sender::send_msg(std::string msg){
	connect(sockfd,serverinfo->ai_addr,serverinfo->ai_addrlen);
	send(sockfd,msg.c_str(),msg.size(),0);
	return 0;
}

int Sender::close_connection(){
	return close(sockfd);
}

void Sender::connect_a(){
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1","25002", &hints, &serverinfo);
	if((sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) == -1)
		return;
	if(connect(sockfd,serverinfo->ai_addr,serverinfo->ai_addrlen) == -1)
		return;
	send(sockfd,"ay quit\n",9,0);
	close(sockfd);
}
