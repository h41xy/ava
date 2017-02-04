#include "node.h"

Node::Node(const int node_id)
	: book(ADDRESSFILE)
	  , id(node_id)
{
	myself = book.getbyid(node_id);
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

	std::cout << "Node " << this->id << " startet on port " << myself.getport() << std::endl;
	do{
		confd = listener.accept_connection();
		// Construct empty message which gets overwritten by recvd message
		read(confd,&message,sizeof(message));
		close(confd);

	//}while(true);
	}while(process_recvd_msg(message));

	// Node exit

	return -1;
}

bool Node::process_recvd_msg(Message& message){
	// have to get initialised every time, consider make global
	const bool quit_node = false;
	const bool continue_node = true;

	increment_ltime();

	switch(message.get_signal_id()){

		case EXIT_NODE : {
					 return quit_node;
				 }
		case EXIT_ALL : {
					sc_exit_all(message);
					return quit_node;
				}
		case REQUEST : {
				       received_request(message.get_sender().getid(), message.get_ltime());
					return continue_node;
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

int Node::start_request(){
	send_request(this->id,this->ltime);
	request_queue.push(QEntry(this->id,this->ltime));

	return -1;
}

int Node::received_request(int id, int ltimestamp){
	QEntry qentry(id, ltimestamp);
	request_queue.push(qentry);
	send_ack(id);
	return -1;
}

int Node::exit_cs(){
	// In this case it must be me whos on top
	request_queue.pop();
	send_release(this->id);
	return -1;
}

int Node::received_release(int id){
	request_queue.pop();

	return -1;
}

int Node::received_acknowledge(int id, int ltimestamp){
	Acknowledge new_acknowledge(id, ltimestamp);
	acknowledges[id] = new_acknowledge;

	return -1;
}

int Node::send_release(int id){
	Message new_release(myself, RELEASE, ltime);
	send_all_message(book, new_release);

	return -1;
}

int Node::send_ack(int receiver_id){
	Entry receiver = book.getbyid(receiver_id);
	Message new_acknowledge(myself, ACKNOWLEDGE, this->ltime);

	send_message(receiver, new_acknowledge);

	return -1;
}

int Node::increment_ltime(){
	this->ltime++;
	return -1;
}

int Node::send_request(int id, int ltimestamp){
	Message new_request_msg(myself,REQUEST,ltimestamp);
	send_all_message(book, new_request_msg);
	return -1;
}

// Message handling
int Node::logger_signal_out(Entry& receiver, Message& message, const bool& connection){

	Sender logger(LOGGER_IP, LOGGER_PORT);

	ss << "[NODE_ID: " << this->id << " ]";
	ss << "[LTIME: " << this->ltime << " ]";
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

	ss << "[NODE_ID: " << this->id << " ]";
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
		// this if is dang unefficient but needed so I dont send signals to myself
		if((*it).getid() != this->id)
			send_message((*it), message);
	}while(++it != receivers.end());
	return -1;
}

int Node::send_message(Entry& receiver, Message& message){
	Sender sender(receiver.getip(),receiver.getport());
	if(sender.get_connection() != -1){
		increment_ltime();
		sender.send_message(message);
		logger_signal_out(receiver,message,true);
		sender.close_connection();
		return 0;
	} else {
		logger_signal_out(receiver,message,false);
	}
	return -1;
}
