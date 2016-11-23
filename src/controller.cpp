// Controller to send signals to ports
#include <iostream>
#include <sstream>

#include "sender.h"
#include "listener.h"
#include "constants.h"

// Creates a Sender object on a given port and sends a signal to it (an int as binary)
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
		// TODO send msg, not just signal
		ss << RECV_MSG << ") Send a text message to this node." << std::endl;
		ss << SOCIALISE << ") Send ID to neighbors." << std::endl;
		ss << RUMOR << ") Spread a rumor." << std::endl;
		std::cout << ss.str();
		std::cin >> signal;
	}
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Listener listener(WATCHER_PORT);
	listener.create_and_listen();
	Sender sender("localhost",port);
	sender.get_connection();
	sender.send_signalid(signal);
	sender.close_connection();
	int confd, msg_id;
	bool listen_more = true;
	do{ 
		msg_id = -1;
		confd = listener.accept_connection();
		read(confd,&msg_id,sizeof(msg_id));

		// Only receivable signals are RECV_MSG so break if else
		if(msg_id != RECV_MSG)
			break;

		char a[MSG_BUFFER_SIZE];
		memset(&a[0],0,sizeof(a));
		read(confd,&a,sizeof(a));
		std::cout << a;
	}while(listen_more);
}
