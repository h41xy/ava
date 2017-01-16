#include "n_rumor.h"

// Calls super constructor
N_rumor::N_rumor(char* id_cstr, char* belive_border_cstr) : Node(id_cstr) {

	// Args get passed as cstring so I am converting first to String and then to int
	std::string believe_border_str(belive_border_cstr);

	// init starting values
	believe_border = std::stoi(believe_border_str);
	heard_rumor = false;
	believe_rumor = false;
}

// Sends a rumor to all neighbors except the one this node heard it from
int N_rumor::send_all_rumor(Addressbook receivers, int sender_id, int signalid){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		// Don't send it to the one I received it
		if((*it).getid() != sender_id){
			Sender sender((*it).getip(),(*it).getport());
			if((sender.get_connection()) != -1){
				sender.send_signalid(signalid);
				sender.send_id(myid);
				sender.close_connection();
				Node::signal_out(it,signalid,true);
			} else {
				Node::signal_out(it,signalid,false);
			}
		}

	}while(++it != receivers.get_end());
	return -1;
}

// Case RUMOR
int N_rumor::sc_rumor(int& confd){
	int sender_id = -1;
	read(confd,&sender_id,sizeof(sender_id));
	rumor_counter++;
	if(!heard_rumor){
		Node::signal_in(RUMOR);
		heard_rumor = true;
		send_all_rumor(neighbors, sender_id, RUMOR);
	}
	if(rumor_counter >= believe_border && !believe_rumor){
		believe_rumor = true;

		Sender sender(WATCHER_IP,WATCHER_PORT);

		std::string ibelieve = "I believe the rumor.";
		if(sender.get_connection() != -1){
			sender.send_msg(vtime, ibelieve);
			sender.close_connection();
			Node::msg_out(WATCHER_IP,WATCHER_PORT,ibelieve,true);
		} else {
			Node::msg_out(WATCHER_IP,WATCHER_PORT,ibelieve,false);
		}
	}
	return -1;
}

// The main loop of the node
// do until receive the exit signal
int N_rumor::run(){

	// Lookup the id from argv and get my associated port
	myself = book.getbyid(myid);
	std::string myip = myself.getip();
	std::cout << "My ID is " << myid << ", my port is: " << myself.getport() << std::endl << "I believe a rumor if heard " << believe_border << " times." << std::endl;


	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop

	// Init values
	int confd = -1;
	bool listen_more = true;
	// The rumor counter has to be initialized, unexpected behaviour is the result otherwise
	rumor_counter = 0;

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
			case RUMOR : {
					     // start spreading a rumor
					     vtime_up(vtimestamp);
					     sc_rumor(confd);
					     break;
				     }
			case PRINT_VTIME : {
						   sc_print_vtime();
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
