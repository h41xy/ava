#include "message.h"

Message::Message() : Message(Entry(), STD_SIGNAL, STD_LTIME, STD_STRING) {}

Message::Message(const int signal_id) : Message(Entry(), signal_id, STD_LTIME, STD_STRING) {}

Message::Message(const Entry sender, const int signal_id, const int ltime) : Message(sender, signal_id, ltime, STD_STRING){}

Message::Message(const Entry sender, const int signal_id, const int ltime, std::string msg)
	: sender(sender)
	, signal_id(signal_id)
	, ltime(ltime)
{
	std::strcpy(this->msg, msg.c_str());
	msg_id = get_random(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
}

unsigned int Message::get_random(const unsigned int& min, const unsigned int& max){
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dis(min,max);
	return dis(rng);
}

const Entry& Message::get_sender() const{
	return this->sender;
}

const int& Message::get_signal_id() const{
	return this->signal_id;
}

const unsigned int& Message::get_msg_id() const{
	return this->msg_id;
}

const int& Message::get_ltime() const{
	return this->ltime;
}

const std::string Message::get_msg() const{
	return std::string(this->msg);
}

int Message::set_msg_id(unsigned int new_msg_id){
	this->msg_id = new_msg_id;
	return -1;
}

int Message::set_msg(std::string new_msg){
	std::strcpy(this->msg, new_msg.c_str());
	return -1;	
}
