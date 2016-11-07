#include <iostream>
#include <cerrno>

#include "sender.h"

int main(){
	Sender sender("localhost", 25002);
	//sender.connect_a();
	if (sender.get_socket() == -1)
		std::cout << "The socket creation failed.\n";
	if ( sender.get_connection() == -1)
		std::cout <<"The connect failed.\n";
	sender.send_msg("Ayyyyyy. quit\n");
	sender.close_connection();
	std::cout << std::strerror(errno) << "\n";
}
