#include <iostream>

#include "sender.h"
#include "constants.h"

int main(int argc, char* argv[]){
	int port, signal;
	std::cout << "Welcher Port? ";
	std::cin >> port;
	std::cout << "Welches Signal?" << std::endl << EXIT_ALL << ") Exit all nodes." << std::endl << RECV_MSG << ") Send a text message to this node." << std::endl << SOCIALISE << ") Send ID to neighbors." << std::endl << RUMOR << ") Spread a rumor." << std::endl;
	std::cin >> signal;
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Sender sender("localhost",port);
	sender.get_connection();
	sender.send_signalid(signal);
	sender.close_connection();
}
