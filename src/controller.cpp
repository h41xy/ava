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
	int port = -1, signal = -1, watcher = -1;
	bool start_watcher = false;
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
	Listener listener(WATCHER_PORT);
	if(start_watcher){
		listener.create_and_listen();
	}
	std::cout << "Sending signal " << signal << " to port " << port << std::endl;
	Sender sender("localhost",port);
	sender.get_connection();
	sender.send_message(Message(Entry(0,"0.0.0.0",25000), signal, 0, 0, ""));
	sender.close_connection();
	if(start_watcher){
		int confd, msg_id, believing_counter = 0;
		bool listen_more = true;
		std::ostringstream rumorresponses;
		do{ 
			confd = listener.accept_connection();

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
		ofs.open(RESULTFILE, std::ios_base::app | std::ios_base::out);
		ofs << rumorresponses.str();
		ofs.close();
		listener.close_socket();
	}
}
