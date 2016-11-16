#include <iostream>
#include <sstream>

#include "sender.h"
#include "constants.h"

int main(int argc, char* argv[]){
	int port = -1, signal = -1;
	if( argc >= 2 ){
	std::string port_str(argv[1]);
	port = std::stoi(port_str);
	}
	if(argc >= 3){
		std::string signal_str(argv[2]);
		signal = std::stoi(signal_str);
	}
	if (port == -1) {
		std::cout << "Welcher Port? ";
		std::cin >> port;
	}
	if (signal == -1){
		std::stringstream ss;
		ss << "Welches Signal?" << std::endl;
		ss << EXIT_ALL << ") Exit all nodes." << std::endl;
		ss << RECV_MSG << ") Send a text message to this node." << std::endl;
		ss << SOCIALISE << ") Send ID to neighbors." << std::endl;
		ss << RUMOR << ") Spread a rumor." << std::endl;
		std::cout << ss.str();
		std::cin >> signal;
	}
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Sender sender("localhost",port);
	sender.get_connection();
	sender.send_signalid(signal);
	sender.close_connection();
}
