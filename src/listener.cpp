#include "listener.h"

Listener::Listener(std::string ip, int port){
	//-------getaddrinfo() localhost--------------
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //localhost

	getaddrinfo(NULL, "25001", &hints, &serverinfo); //NULL is for localhost in comb with AI_PASSIVE flag
	//--------------------------------------------------
	
	//--------socket()----------------------------------
	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	//--------------------------------------------------
	
	//----bind()----
	bind(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
	//--------------

	//----listen()----
	listen(sockfd,10);
	//----------------

	//----accept()----
	addr_size = sizeof peer_addr;
	while(1){
		confd = accept(sockfd, (struct sockaddr *)&peer_addr, &addr_size);
		std::string str;
		do{
			memset(buffer,0,sizeof buffer);
			recv(confd, buffer, 256, 0);
			str = std::string(buffer);
			std::cout << str;
		}while(1);
		close(confd);
		break;
	}
	//----------------
}
