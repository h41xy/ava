#include "node.h"

Node::Node(const int node_id)
	: book(ADDRESSFILE)
	, myid(node_id)
{
	myself = book.getbyid(myid);
}

// The main loop of the node
// do until receive the exit signal
int Node::run(){

	// listen on the port
	Listener listener(myself.getport());
	listener.create_and_listen();

	listen_loop(listener);

	listener.close_socket();
	std::cout << std::strerror(errno) << "\n";

	return -1;
}

int Node::listen_loop(Listener& listener){
	// Init values
	int confd = -1;
	Message message;

	do{
		confd = listener.accept_connection();
		// Construct empty message which gets overwritten by recvd message
		read(confd,&message,sizeof(message));
		close(confd);

	}while(process_recvd_msg(message));

	// Node exit

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
						sc_exit_all(message);
						return quit_node;
					}

			default :
					// received unknown signal
					   return continue_node;
		}

	return continue_node;
}

// Switch case methods

// Case EXIT_ALL
int Node::sc_exit_all(Message& message){
	Message exit(EXIT_ALL);
	send_all_message(book, exit);
	return -1;
}

// Message handling
int Node::logger_signal_out(Entry& receiver, Message& message, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << myid << " ]";
	ss << "[MType: OUT]";
	ss << "[Recv_ID: " << receiver.getid() << " ]";
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;
	ss << "[Send: ";

	if (connection) {
		ss << "SUCCESS]";
	} else {
		ss << "FAILED]";
	}

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, ss.str()));
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
	ss << "[SenderID: " << message.get_sender().getid() << " ]";
	ss << "[S_ID: " << message.get_signal_id() << "]";;
	ss << "[M_ID: " << std::hex << message.get_msg_id() << " ]";
	ss << std::dec;

	if (logger.get_connection() != -1) {
		ss << std::endl;
		logger.send_message(Message(myself, RECV_MSG, ss.str()));
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
	std::list<Entry>::iterator it = receivers.begin();
	do{
		send_message((*it), message);
	}while(++it != receivers.end());
	return -1;
}

int Node::send_message(Entry& receiver, Message& message){
	Sender sender(receiver.getip(),receiver.getport());
	if(sender.get_connection() != -1){
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
		return 0;
	} else {
		logger_signal_out(receiver,message,false);
	}
	return -1;
}
