#include "n_voter.h"

N_voter::N_voter(char* id_cstr) : Node(id_cstr){
	candidates = Addressbook(ADDRESSFILE, get_candidate_ids(CANDIDATEFILE));
	candidate_count  = candidates.entrycount();

	neighbors_wo_candidates = neighbors;
	eliminate_candidates_from_neighbors(neighbors_wo_candidates, candidates);

	// random generator

	for (int i = 1; i<=candidate_count; i++){
		candidate_c_levels[i] = get_random(100, 0);
	}

}


// I know the candidates addressbook is not needed, its in there for when the candidate ids get complex
int N_voter::eliminate_candidates_from_neighbors(Addressbook& neighbors_wo_candidates, Addressbook& candidates){
	for (int i=1; i<=candidate_count; i++){
		neighbors_wo_candidates.remove(i);
	}
	return -1;
}

// Get the Candidate IDs from a file
// TODO the whole, is it a file etc checks
std::list<int> N_voter::get_candidate_ids(const std::string& fname){
	std::ifstream ifile;
	ifile.open(fname);
	std::list<int> candidate_ids;
	std::string line;
	int c_id;

	while(getline(ifile,line)){
		c_id = std::stoi(line);
		candidate_ids.push_back(c_id);
	}
	ifile.close();
	return candidate_ids;
}

int N_voter::vote_me_response(Message& inc_message){

	// from which candidate originated teh msg
	int candidate_id = inc_message.get_origin();

	// raise c lvl based on senders clvl
	int sender_clvl = inc_message.get_sender_clvl();

	candidate_c_levels[candidate_id] += (sender_clvl / 10);

	Message message(myself, VOTE_ME, candidate_id, candidate_c_levels[candidate_id], "");
	message.set_msg_id(inc_message.get_msg_id());

	// check c_lvl and respond
	bool doublemax = false;
	int max_id = 0;
	find_id_of_max_value(candidate_c_levels, max_id, doublemax);

	// if candidateorigin is my fav candidate
	if (inc_message.get_origin() == max_id) {
		// send him a KEEP_ON
		// TODO let this method return a pointer
		Entry candidate_entry = candidates.getbyid(candidate_id);
		send_signal(candidate_entry, KEEP_ON);

		// if max value is not shared with another candidate
		if (!doublemax){
			// send all neighbors a VOTE_ME signal with candidate origin id and my c lvl
			std::list<Entry>::iterator it = neighbors_wo_candidates.get_iterator();
			do{
				// all except the sender
				if ((*it).getid() != message.get_sender().getid()){
					Sender sender((*it).getip(),(*it).getport());
					if((sender.get_connection()) != -1){
						vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
						sender.send_message(message);
						logger_signal_out((*it),message,true);
						sender.close_connection();
					} else {
						logger_signal_out((*it),message,false);
					}

				}
			}while(++it != neighbors_wo_candidates.get_end());

		}

	} else {
		// else send him a NOT_YOU
		// TODO let this method return a pointer
		Entry candidate_entry = candidates.getbyid(candidate_id);
		vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
		send_signal(candidate_entry, NOT_YOU);
		vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
	}
	//TODO msg in an out
	return -1;
}

int N_voter::find_id_of_max_value(std::map<int,int>& candidate_c_levels, int& max_id, bool& doublemax){
	int max = 0; // max value
	for (int i =1; i<=candidate_count; i++){
		if (candidate_c_levels[i] >= max){
			if (doublemax) {
				doublemax = false;
			}
			if (candidate_c_levels[i] == max){
				doublemax = true;
			}
			max = candidate_c_levels[i];	
			max_id = i;
		}
	}
	return -1;
}

int N_voter::init_as_partybuddy(Message& message){
	for (int i = 1; i <= candidate_count; i++){
		if ( candidate_c_levels[i] == message.get_origin()) {
			candidate_c_levels[i] = 100;
		} else {
			candidate_c_levels[i] = 0;
		}
	}
	return -1;
}

int N_voter::v_process_echo_explore(Message& explore){

	// if echo_identifier[explorer.get_msg_id()] == empty
	if (echo_identifier.count(explore.get_msg_id()) == 0){
		// 	create new struct
		// 	insert in map
		echo_identifier[explore.get_msg_id()] = Echo_content();
	}

	// Struct current = map[msg_id]
	Echo_content* current = &echo_identifier[explore.get_msg_id()];
	// all following but with ref to the cur struct

	(*current).echo_counter++;
	// if state == white
	if ((*current).state == white){
		if(!vtime_terminated){
			// process clvls
			bool doublemax = false;
			int max_id = 0;
			find_id_of_max_value(candidate_c_levels, max_id, doublemax);

			// if max values are unequal
			if (!doublemax){
				// whoever originates the echo, all but the fav candidate get --
				for (int i=1; i<candidates.entrycount(); i++){
					if ( i == max_id ){
						candidate_c_levels[i]++;
					} else {
						candidate_c_levels[i]--;
					}
				}
			}

		}
		(*current).state = red;
		// send explore to neighbors (except sender)
		Message new_explore(myself, ECHO_EXPLORE, explore.get_origin(), 100, "");
		new_explore.set_msg_id(explore.get_msg_id());
		std::list<Entry>::iterator it = neighbors.get_iterator();
		do{
			if ((*it).getid() != explore.get_sender().getid()){
				vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
				send_message((*it), new_explore);
			}
		}while(++it != neighbors.get_end());
		// remember sender
		(*current).first_neighbor = explore.get_sender();

	}
	if ((*current).echo_counter == neighbors.entrycount()){
		(*current).state = green;
		Message echo(myself, ECHO_EXPLORE, myself.getid(), 100, "");
		echo.set_msg_id(explore.get_msg_id());
		vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
		send_message((*current).first_neighbor, echo);
		// reset map entry
		(*current) = Echo_content();
	}

	return -1;
}


int N_voter::vtime_check_terminate(std::vector<int>& cur_vtime, std::vector<int>& term_vtime, bool& vtime_terminated){
	//if (cur_vtime >= term_vtime)
	if (cur_vtime[myid - 1] >= term_vtime[myid - 1]){
		if (!vtime_terminated){
			// TODO no time to explain, get to da chopper
			std::ostringstream os;
			// TODO hardcoded : /
			os << "[C_ID:LVL 1:" << candidate_c_levels[1] << " " << "2:" << candidate_c_levels[2] << " ]";
			os << "[TermTime: " << vtime[myid -1] << " ]";
			Message term_me(myself,NODE_TERMINATED,-1,-1,os.str());
			logger_signal_out(myself, term_me, true); 
		}
		vtime_terminated = true;
	}
	return -1;
}

int N_voter::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	// TODO standartize output
	std::cout << "[NODE_ID: " << myid << " ]";
	std::cout << "[NType: VOTER]";
	std::cout << "[State: STARTED]";
	std::cout << "[Neighbors: ";
	std::list<Entry>::iterator it = neighbors_wo_candidates.get_iterator();
	do{
		std::cout << " " << (*it).getid();
	}while(++it != neighbors_wo_candidates.get_end());
	std::cout << " ]";

	std::cout << "[Clvls";
	for(int i = 1; i<=candidate_count; i++){
		std::cout << " " << i << ":" << candidate_c_levels[i];
	}
	std::cout << " ]";
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
			case VOTE_ME : {
					       vtime_up(vtimestamp);
						if (vtime_terminated)
							break;
					       if (std::find(known_signals.begin(), known_signals.end(), message.get_msg_id()) != known_signals.end()){
						       // TODO some kind of rejected message
						       logger_signal_in(message);
						       break;
					       } else {
						       known_signals.push_back(message.get_msg_id());
						       logger_signal_in(message);
						       vote_me_response(message);
					       }
						vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
					       break;
				       }
			case ECHO_EXPLORE : {
							// cant break here on vtime terminated reached, the network gets a deadlock
							// clvl changes get ignored in the following methods
						    vtime_up(vtimestamp);
						    logger_signal_in(message);
						    v_process_echo_explore(message);
						vtime_check_terminate(vtime,vtime_to_terminate,vtime_terminated);
						    break;
					    }
			case INIT_PB : {
					       init_as_partybuddy(message);
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
