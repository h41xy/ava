#include "n_voter.h"

N_voter::N_voter(char* id_cstr) : Node(id_cstr){
	candidates = Addressbook(ADDRESSFILE, get_candidate_ids(CANDIDATEFILE));
	candidate_count  = candidates.entrycount();

	std::srand(std::time(0)); // seed for random
	for (int i = 1; i<=candidate_count; i++){
		candidate_c_levels[i] = get_random(100, 0);
	}
}

// returns a random value in the given range
int N_voter::get_random(const int& max, int min){

	if (min > max)
		return 0;

	if(max <= 0)
		return 0;

	if(min <= 0)
		min = 0;

	return (std::rand() % max) + min;
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

int N_voter::vote_me_response(const int& confd){

	// from which candidate originated teh msg
	int candidate_id = 0;
	read(confd,&candidate_id,sizeof(candidate_id));

	// raise c lvl based on senders clvl
	int sender_clvl = 0;
	read(confd,&sender_clvl,sizeof(sender_clvl));

	candidate_c_levels[candidate_id] += (sender_clvl / 10);

	// check c_lvl and respond
	bool doublemax = false;
	int max_id = 0;
	find_id_of_max_value(candidate_c_levels, max_id, doublemax);

	// if candidateorigin is my fav candidate
	if (sender_entry.getid() == max_id) {
		// send him a KEEP_ON
		// TODO let this method return a pointer
		Entry candidate_entry = candidates.getbyid(candidate_id);
		send_signal(candidate_entry, KEEP_ON);

		// if max value is not shared with another candidate
		if (!doublemax){
			// send all neighbors a VOTE_ME signal with candidate origin id and my c lvl
			std::list<Entry>::iterator it = neighbors.get_iterator();
			do{
				Sender sender((*it).getip(),(*it).getport());
				if((sender.get_connection()) != -1){
					sender.send_entry(myself);
					sender.send_signalid(VOTE_ME);
					sender.send_id(candidate_id); // candidate id
					sender.send_id(candidate_c_levels[candidate_id]); // my clvl of this candidate
					Node::signal_out((*it),VOTE_ME,true);
					sender.close_connection();
				} else {
					Node::signal_out((*it),VOTE_ME,false);
				}

			}while(++it != neighbors.get_end());

		}

	} else {
		// else send him a NOT_YOU
		// TODO let this method return a pointer
		Entry candidate_entry = candidates.getbyid(candidate_id);
		send_signal(candidate_entry, NOT_YOU);
	}
	//TODO msg in an out
	return -1;
}

int N_voter::find_id_of_max_value(std::map<int,int>& candidate_c_levels, int& max_id, bool& doublemax){
	int max = 0; // max value
	int c_id_max = 0; // id with max value
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

int N_voter::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "I'm a voter. \nMy ID is " << myid << ", my port is: " << myself.getport() << std::endl;
	for(int i = 1; i<=candidate_count; i++){
		std::cout << "Confidence level for candidate " << i << " is " << candidate_c_levels[i] << "\n";
	}


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

		// read out sender entry
		read(confd,&sender_entry,sizeof(sender_entry));
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
					       break;
				       }
			case VOTE_ME : {
						vtime_up(vtimestamp);
						vote_me_response(confd);
					       break;
				       }
			case KEEP_ON : {
// Signal is only for debugging or init reasons in the switch case
					       break;
				       }
			case NOT_YOU : {
// Signal is only for debugging or init reasons in the switch case
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
