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
	int port = -1, signal = -1, watcher = -1, logger = -1;
	bool start_watcher = false;
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
		std::string watcher_str(argv[3]);
		watcher = std::stoi(watcher_str);
		if(watcher == 0)
			start_watcher = true;
	}
	if(argc >= 5){
		std::string logger_str(argv[4]);
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
		ss << "Welches Signal?" << std::endl;
		ss << EXIT_ALL << ") Exit all nodes." << std::endl;
		ss << VOTE_ME << ") starts a vote me only if the addressant is a candidate.\n";
		ss << CAMPAIGN << ") starts a campaign only if the addressant is a candidate.\n";
		ss << INIT << ") inits the network.\n";
		ss << SET_TERMINATE_VTIME_ALL << ") sets a termination time for all.\n";
		std::cout << ss.str();
		std::cin >> signal;
	}
	if (watcher == -1){
		int i = -1;
		std::cout << "Start watcher? 0 is yes ";
		std::cin >> i;
		if(i==0)
			start_watcher = true;
	}
	if (logger == -1){
		int i = -1;
		std::cout << "Start logger? 0 is yes ";
		std::cin >> i;
		if(i==0)
			start_logger = true;
	}
	if (start_watcher && start_logger){
		std::cout << "You can't start both, logger and watcher, in the same process.\n";
		return -1;
	}
	Listener* listenerp;
	Listener listener_watcher(WATCHER_PORT);
	Listener listener_logger(LOGGER_PORT);
	
	if(start_watcher){
		listenerp = &listener_watcher;
		(*listenerp).create_and_listen();
		std:: cout << "Watcher started on Port " << WATCHER_PORT << "\n";
	}
	if(start_logger){
		listenerp = &listener_logger;
		(*listenerp).create_and_listen();
		std:: cout << "Logger started on Port " << LOGGER_PORT << "\n";
	}
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Sender sender("localhost",port);
	if(sender.get_connection() == 0){
		sender.send_message(Message(Entry(0,"0.0.0.0",25000), signal, 0, 0, ""));
	}
	sender.close_connection();
	if(start_watcher || start_logger){
		int confd, msg_id, believing_counter = 0;
		bool listen_more = true;
		std::ostringstream rumorresponses;
		do{ 
			confd = (*listenerp).accept_connection();

			Message message(Entry(0,"",0),-1,-1,-1,"");
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
		if(start_watcher){
			ofs.open(RESULTFILE, std::ios_base::app | std::ios_base::out);
		}
		if(start_logger){
			ofs.open(LOGGERFILE, std::ios_base::app | std::ios_base::out);
		}
		ofs << rumorresponses.str();
		ofs.close();
		(*listenerp).close_socket();
	}
}
