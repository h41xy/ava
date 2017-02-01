// The node
// TODO rework initialization
#include "node.h"

Node::Node(const int node_id)
	: myid(node_id)
	, book(ADDRESSFILE)
{
	myself = book.getbyid(myid);
}

// The main loop of the node
// do until receive the exit signal
int Node::run(){

	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	// listener loop
	listen_loop(listener);

	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}

int Node::listen_loop(Listener& listener){
	// Init values
	int confd = -1;

	do{
		confd = listener.accept_connection();
		// Receive msgs and react to them
		// Construct empty message which gets overwritten by recvd message
		Message message();
		read(confd,&message,sizeof(message));
		close(confd);

	}while(process_recvd_msg(message));

	std::cout << "ID: " << myid << " Node exited." << std::endl;

	return -1;
}

int Node::process_recvd_msg(Message& message){
	// have to get initialised every time, consider make global
	const bool quit_node = false;
	const bool continue_node = true;

		switch(message.get_signal_id()){

			case EXIT_NODE : {
						 return quit_node;
					 }
			case EXIT_ALL : {
						sc_exit_all(message, listen_more);
						return quit_node;
					}

			default :
					// received unknown signal
					   return continue_node;
		}

	return continue_node;
}

// Message handling
int Node::logger_signal_out(Entry& receiver, Message& message, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << myid << " ]";
	ss << "[MType: OUT]";
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[V_TIME: " << vtime[myid - 1] << " ]";
	ss << "[Recv IP/Port: " << receiver.getip() << "/" << receiver.getport() << " ]";
	ss << message.get_msg();
	ss << "[Send: ";

	if (connection) {
		ss << "SUCCESS]";
	} else {
		ss << "FAILED]";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, 0, 0, ss.str()));
		logger.close_connection();
	} else {
		ss << "[LoggerCon FAILED]";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

int Node::logger_signal_in(Message& message){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << myid << " ]";
	ss << "[MType: IN]";
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[V_TIME: " << vtime[myid - 1] << " ]";
	ss << "[SenderID: " << message.get_sender().getid() << " ]";
	ss << message.get_msg();

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, 0, 0, ss.str()));
		logger.close_connection();
	} else {
		ss << "[LoggerCon FAILED]";
		std::cout << ss.str() << std::endl;
	}

	clear_stringstream(ss);
	return -1;
}

// Deletes the content in the stringstream
int Node::clear_stringstream(std::stringstream& ss){
	ss.str(std::string());
	ss.clear();
	return -1;
}

int Node::send_all_message(Addressbook& receivers, Message& message){
	std::list<Entry>::iterator it = receivers.get_iterator();
	do{
		send_message((*it), message);
	}while(++it != receivers.get_end());
	return -1;
}

int Node::send_message(Entry& receiver, Message& message){
	vtime_up(vtime);
	message.set_vtimestamp(vtime);
	Sender sender(receiver.getip(),receiver.getport());
	if(sender.get_connection() != -1){
		// TODO rework message class
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
		return 0;
	} else {
		logger_signal_out(receiver,message,false);
	}
	return -1;
}

// Switch case methods

// Case EXIT_NODE
int Node::sc_exit_node(Message& message, bool& listen_more){
	logger_signal_in(message);
	listen_more = false;
	return -1;
}

// Case EXIT_ALL
int Node::sc_exit_all(Message& message, bool& listen_more){
	logger_signal_in(message);
	Message new_message(message.get_sender(),EXIT_NODE,message.get_origin(), message.get_sender_clvl(),message.get_msg());
	send_all_message(book, new_message);
	listen_more = false;
	return -1;
}
