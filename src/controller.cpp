// Controller to send signals to ports
#include <iostream>
#include <sstream>
#include <fstream>

#include "sender.h"
#include "listener.h"
#include "constants.h"
#include "entry.h"
#include "message.h"

// Creates a Sender object on a given port and sends a signal to it (an int as binary)
int main(int argc, char* argv[]){
	int port = -1, signal = -1, logger = -1;
	bool start_logger = false;
	if( argc >= 2 ){
	std::string port_str(argv[1]);
	port = std::stoi(port_str);
	}
	if(argc >= 3){
		std::string signal_str(argv[2]);
		signal = std::stoi(signal_str);
	}
	if(argc >= 4){
		std::string logger_str(argv[3]);
		logger = std::stoi(logger_str);
		if(logger == 0)
			start_logger = true;
	}
	if (port == -1) {
		std::cout << "Welcher Port? ";
		std::cin >> port;
	}
	if (signal == -1){
		std::stringstream ss;
		ss << "Signal to send?" << std::endl;
		ss << EXIT_ALL << ") Exit all nodes." << std::endl;
		std::cout << ss.str();
		std::cin >> signal;
	}
	if (logger == -1){
		int i = -1;
		std::cout << "Start logger? 0 is yes ";
		std::cin >> i;
		if(i==0)
			start_logger = true;
	}
	Listener* listenerp;
	Listener listener_logger(LOGGER_PORT);
	
	if(start_logger){
		listenerp = &listener_logger;
		(*listenerp).create_and_listen();
		std:: cout << "Logger started on Port " << LOGGER_PORT << "\n";
	}
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Sender sender("localhost",port);
	if(sender.get_connection() == 0){
		sender.send_message(Message(signal));
	}
	sender.close_connection();
	if(start_logger){
		int confd, msg_id, believing_counter = 0;
		bool listen_more = true;
		std::ostringstream rumorresponses;
		do{ 
			confd = (*listenerp).accept_connection();

			Message message;
			read(confd,&message,sizeof(message));
			msg_id = message.get_signal_id();

			if(msg_id == EXIT_NODE || msg_id == EXIT_ALL)
				listen_more = false;
			// Only receivable signals are RECV_MSG so break if else
			if(msg_id != RECV_MSG)
				break;

			//char a[MSG_BUFFER_SIZE];
			//memset(&a[0],0,sizeof(a));
			//a = message.get_msg();
			std::string a(message.get_msg());
			std::cout << a;
			rumorresponses << a;
			believing_counter++;
			close(confd);
		}while(listen_more);
		std::ofstream ofs;
		if(start_logger){
			ofs.open(LOGGERFILE, std::ios_base::app | std::ios_base::out);
		}
		ofs << rumorresponses.str();
		ofs.close();
		(*listenerp).close_socket();
	}
}
