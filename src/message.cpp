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
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min,max);
	return dis(gen);
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

const std::string Message::get_msg() const{
	return std::string(this->msg);
}
