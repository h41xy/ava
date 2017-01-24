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
	// vtime ++
	vtime[myid -1] = vtime[myid - 1] ++;
	return -1;
}

int N_candidate::campaign(){
	Message new_explorer(myself,ECHO_EXPLORE, myself.getid(), 100, "");
	// keep track of my echo algorithms
	echo_id_list.push_back(new_explorer.get_msg_id());
	echo_identifier[new_explorer.get_msg_id()] = Echo_content();
	echo_identifier[new_explorer.get_msg_id()].state = red;
	send_all_message(neighbors, new_explorer);

	// vtime ++
	vtime[myid -1] = vtime[myid - 1] ++;
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
		if ( get_random(2,1) % 2 == 0 ){
			vote_me();
		} else {
			campaign();
		}
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

int N_candidate::c_process_echo_explore(Message& explore){

	// if echo_identifier[explorer.get_msg_id()] == empty
	if (echo_identifier.count(explore.get_msg_id()) == 0){
	// 	create new struct
	// 	insert in map
		echo_identifier[explore.get_msg_id()] = Echo_content();
	}

	// Struct current = map[msg_id]
	// Watch out, this must be a pointer ; )
	Echo_content* current = &echo_identifier[explore.get_msg_id()];
	// all following but with ref to the cur struct

	(*current).echo_counter++;
	// if state == white
	if ((*current).state == white){
		(*current).state = red;
		// send explore to neighbors (except sender)
		Message new_explore(myself, ECHO_EXPLORE, myself.getid(), 100, "");
		new_explore.set_msg_id(explore.get_msg_id());
		std::list<Entry>::iterator it = neighbors.get_iterator();
		do{
			if ((*it).getid() != explore.get_sender().getid())
				send_message((*it), new_explore);
		}while(++it != neighbors.get_end());
		// remember sender
		(*current).first_neighbor = explore.get_sender();
	}
	if ((*current).echo_counter == neighbors.entrycount()){
		(*current).state = green;
		Message echo(myself, ECHO_EXPLORE, myself.getid(), 100, "");
		echo.set_msg_id(explore.get_msg_id());
		send_message((*current).first_neighbor, echo);

		// Remove the returned msg id from the stack
		echo_id_list.erase(std::find(std::begin(echo_id_list), std::end(echo_id_list), explore.get_msg_id()));
	}
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
						logger_signal_in(message);
						campaign();
					       break;
				       }
			case VOTE_ME : {
// Signal is only for debugging or init reasons in the switch case
					       vote_me();
					       break;
				       }
			case KEEP_ON : {
						if (vtime_terminated)
							break;
					       vtime_up(vtimestamp);
						logger_signal_in(message);
					       sc_keep_on();
					       break;
				       }
			case NOT_YOU : {
						if (vtime_terminated)
							break;
					       vtime_up(vtimestamp);
						logger_signal_in(message);
					       sc_not_you();
					       break;
				       }
			case ECHO_EXPLORE : {
						if (vtime_terminated)
							break;
						    vtime_up(vtimestamp);
						    logger_signal_in(message);
						    c_process_echo_explore(message);
						    break;
					    }
			case INIT : {
						// do all init work
						init_partybuddies();
						vote_me();
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
