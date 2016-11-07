#include <iostream>
#include <cerrno>

#include "sender.h"

int main(){
	Sender sender("localhost", 25002);
	//sender.connect_a();
	sender.get_socket();
	//sender.get_connection();
	sender.send_msg("Ayyyyyy. quit");
	sender.close_connection();
	std::cout << std::strerror(errno) << "Connection closed.\n";
}
