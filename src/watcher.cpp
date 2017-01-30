#include "watcher.h"

Watcher::Watcher(char* id_cstr, int vtime_to_respond) : Node(id_cstr) {

	// init starting values
	myid = 24000;
	clear_stringstream(ss);

	this->vtime_to_respond = vtime_to_respond;
	responsecounter =  0;
}

int Watcher::run(){
	// prepare
	myself = Entry(24000,"localhost",24000);
	Listener listener(myself.getport());
	listener.create_and_listen();
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
	// init responses
	//	-> request_response_on_vtime();
case INIT_REQUEST_RESPONSES : {
	request_response_on_vtime(message.get_sender_clvl());
	break;
}
	// recv responses
	// 	-> process_recvd_response();
case RESPONSE : {
	process_recvd_response(message);
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

int Watcher::request_response_on_vtime(int vtime_to_respond){
	// Construct message with vtime to respond and Signal
	Message response_request(myself,INIT_REQUEST_RESPONSES,myid,vtime_to_respond,"");
	// send Message
	std::list<Entry>::iterator it = book.get_iterator();
	do{
		// on success, responsecounter++;
		if (send_message((*it),  response_request) == 0)
			responsecounter++;;
	}while(++it != book.get_end());

	return -1;
}

int Watcher::process_recvd_response(Message& message){
	// check if response is successful
	// get candidate id and increase clvl by recvd value
	candidates_c_sums[message.get_origin() - 1]++;
	// succesful response decrements responsecounter
	responsecounter--;
	// if responsecounter reaches 0
	//	-> process_all_responses();
	if (responsecounter <= 0)
		process_all_responses();
	return -1;
}

int Watcher::process_all_responses(){
	// construct string
	//	-> write_to_file(std::string);
	std::ostringstream os;
	if (candidates_c_sums[0] < candidates_c_sums[1]){
		os << "Candidate 1 won with " << candidates_c_sums[0] << " Votes.\n";
	} else {
		os << "Candidate 2 won with " << candidates_c_sums[1] << " Votes.\n";
	}
	write_to_file(os.str());
	return -1;
}

int Watcher::write_to_file(std::string result){
	// all file writeing logic for a string
	std::ofstream ofs;
	ofs.open(WATCHERFILE, std::ios_base::app | std::ios_base::out);
	ofs << result;
	ofs.close();

	return -1;
}
