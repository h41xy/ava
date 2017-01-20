#include "n_candidate.h"

N_candidate::N_candidate(char* id_cstr, char* response_border_cstr) : Node(id_cstr) {
	std::string response_border_str(response_border_cstr);
	response_border = std::stoi(response_border_str);

	response_count = 0;
}

// Sends the VOTE_ME signal to all neighbors
// packs the own candidateid and the senderid
// TODO abstract method or something
int N_candidate::vote_me(){
	std::list<Entry>::iterator it = neighbors.get_iterator();
	Message message(myself, VOTE_ME, myself.getid(), 100, "");
	do{
		Sender sender((*it).getip(),(*it).getport());
		if((sender.get_connection()) != -1){
			sender.send_message(message); //candidate confidence is always 100

			logger_signal_out((*it),message,true);
			sender.close_connection();
		} else {
			logger_signal_out((*it),message,false);
		}

	}while(++it != neighbors.get_end());
	return -1;
}

int N_candidate::campaign(){
	return -1;
}

int N_candidate::sc_keep_on(){
	recv_response();
	return -1;
}

int N_candidate::sc_not_you(){
	recv_response();
	return -1;
}

int N_candidate::recv_response(){
	response_count += 1;
	if (response_count >= response_border) {
		// TODO random start vote_me() || campaign()
		response_count = 0;
	}
	return -1;
}

int N_candidate::init_partybuddies(){
	// send signal to partybuddies with origin me and clvl 100
	Message message(myself,INIT_PB,myself.getid(),100,"");
	send_all_message(neighbors, message);
	return -1;
}

int N_candidate::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "[NODE_ID: " << myid << " ]";
	std::cout << "[NType: CANDIDATE]";
	std::cout << "[State: STARTED]";
	std::cout << std::endl;

	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop

	// Init values
	int confd = -1;
	bool listen_more = true;

	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		Message message(Entry(-1,"",-1), -1, -1, -1, "");

		read(confd,&message,sizeof(message));

		// Vectortimestamp from active connection
		std::vector<int> vtimestamp;
		vtimestamp.resize(book.entrycount());
		std::fill(vtimestamp.begin(),vtimestamp.end(),0);
/*
		// TODO Thats so unsafe its insane...
		for(int i = 0; i < vtimestamp.size(); i++){
			read(confd,&vtimestamp[i],sizeof(int));
		}
*/
		switch(message.get_signal_id()){

			case EXIT_NODE : {
						 // exit single node
						 vtime_up(vtimestamp);
						 sc_exit_node(message, listen_more);
						 break;
					 }
			case EXIT_ALL : {
						// exit all nodes
						vtime_up(vtimestamp);
						sc_exit_all(message, listen_more);
						break;
					}
			case PRINT_VTIME : {
						   sc_print_vtime();
						   break;
					   }
			case CAMPAIGN : {
// Signal is only for debugging or init reasons in the switch case
					       campaign();
					       break;
				       }
			case VOTE_ME : {
// Signal is only for debugging or init reasons in the switch case
					       vote_me();
					       break;
				       }
			case KEEP_ON : {
					       vtime_up(vtimestamp);
						logger_signal_in(message);
					       sc_keep_on();
					       break;
				       }
			case NOT_YOU : {
					       vtime_up(vtimestamp);
						logger_signal_in(message);
					       sc_not_you();
					       break;
				       }
			case INIT : {
						// do all init work
						init_partybuddies();
						break;
					}

			default :
				       std::cout << "ID: " << myid << "I don't know this signal id. Close connection.\n";
				       break;
		}
		close(confd);

	}while(listen_more);
	std::cout << "ID: " << myid << " Node exited." << std::endl;
	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}
