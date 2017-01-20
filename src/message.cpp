#include "message.h"

Message::Message(const Entry& sender, const int& signal_id, const int& origin, const int& clvl, const std::string& msg)
	: sender(sender)
	, signal_id(signal_id)
	, origin(origin)
	  , clvl(clvl)
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

const int& Message::get_origin() const{
	return this->origin;
}

const int& Message::get_sender_clvl() const{
	return this->clvl;
}

const unsigned int& Message::get_msg_id() const{
	return this->msg_id;
}

const std::string Message::get_msg() const{
	return std::string(this->msg);
}

int Message::set_msg_id(unsigned int new_msg_id){
	this->msg_id = new_msg_id;
	return -1;
}

int Message::set_vtimestamp(int vtimestamp[MAX_VECTORTIME_SIZE]){
	return -1;
}
