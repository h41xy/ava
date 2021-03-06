#include "listener.h"

Listener::Listener(int port){
	//-------getaddrinfo() localhost--------------
	std::string s = std::to_string(port);
	char const *pport = s.c_str();

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //localhost

	getaddrinfo(NULL, pport, &hints, &serverinfo); //NULL is for localhost in comb with AI_PASSIVE flag
	//--------------------------------------------------

	//----accept()----
	addr_size = sizeof peer_addr;
	//----------------
}

int Listener::create_and_listen(){

	//--------socket()----------------------------------
	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	// REUSEADDR so the timeout on socketclose gets skipped
	int enable = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	//--------------------------------------------------

	//----bind()----
	bind(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
	//--------------

	//----listen()----
	return listen(sockfd,SOMAXCONN);
	//----------------
}

int Listener::accept_connection(){
	return accept(sockfd, (struct sockaddr *)&peer_addr, &addr_size);
}

int Listener::close_socket(){
	close(sockfd);
	return -1;
}
