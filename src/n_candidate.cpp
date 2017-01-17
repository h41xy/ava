#include "n_candidate.h"

N_candidate::N_candidate(char* id_cstr, char* response_border_cstr) : Node(id_cstr) {
	std::string response_border_str(response_border_cstr);
	response_border = std::stoi(response_border_str);

	response_count = 0;
}

// Sends the VOTE_ME signal to all neighbors
int N_candidate::vote_me(){
	send_all_signal(neighbors,VOTE_ME);
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

int N_candidate::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "I'm a candidate. \nMy ID is " << myid << ", my port is: " << myself.getport() << std::endl;


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
		int msg_id = -1;

		// Read the msgid from an active connection
		read(confd,&msg_id,sizeof(msg_id));

		// Vectortimestamp from active connection
		std::vector<int> vtimestamp;
		vtimestamp.resize(book.entrycount());
		std::fill(vtimestamp.begin(),vtimestamp.end(),0);

		// TODO Thats so unsafe its insane...
		for(int i = 0; i < vtimestamp.size(); i++){
			read(confd,&vtimestamp[i],sizeof(int));
		}

		switch(msg_id){

			case EXIT_NODE : {
						 // exit single node
						 vtime_up(vtimestamp);
						 sc_exit_node(listen_more);
						 break;
					 }
			case EXIT_ALL : {
						// exit all nodes
						vtime_up(vtimestamp);
						sc_exit_all(listen_more);
						break;
					}
			case RECV_MSG : {
						// recv msgs with max length of 256 chars
						// TODO check on length
						vtime_up(vtimestamp);
						sc_recv_msg(confd);
						break;
					}
			case SOCIALISE : {
						 // send a string msg to all my neighbors with my id
						 vtime_up(vtimestamp);
						 sc_socialise();
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
					       sc_keep_on();
					       break;
				       }
			case NOT_YOU : {
					       vtime_up(vtimestamp);
					       sc_not_you();
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
