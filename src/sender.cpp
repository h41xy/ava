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

// Sends a string message on the established connection
// TODO check on 256 char limit
// TODO check if connected
int Sender::send_msg(std::string msg){
	Sender::send_signalid(RECV_MSG);
	write(sockfd,msg.c_str(),msg.size() + 1);
	return -1;
}

// Sends a string message on the established connection
// TODO check on 256 char limit
// TODO check if connected
int Sender::send_msg(std::vector<int>& vtimestamp, std::string msg){
	Sender::send_signalid(RECV_MSG);
	Sender::send_vtimestamp(vtimestamp);
	write(sockfd,msg.c_str(),msg.size() + 1);
	return -1;
}

// Sends a integer in binary to the connection of the object
// TODO check if connected
int Sender::send_signalid(const int& signalid){
	send_int(signalid);
	return -1;
}

// Sends the vtimestamp
// TODO check if connected
int Sender::send_vtimestamp(std::vector<int>& vtimestamp){
	// write every value sequentially to the socket
	// the read on the other side will pick them up the same way and write
	// them in a new vector
	for(int i = 0; i < vtimestamp.size(); i++){
		Sender::send_id(vtimestamp[i]);
	}
	return -1;
}

// TODO check if connected
int Sender::send_id(const int& id){
	send_int(id);
	return -1;
}

// TODO check if connected
int Sender::close_connection(){
	close(sockfd);
	return -1;
}

// TODO check if connected
int Sender::send_int(const int& i){
	write(sockfd,&i,sizeof(i));
	return -1;
}
