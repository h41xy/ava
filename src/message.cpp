#include "message.h"

Message::Message(const Entry& sender, const int& signal_id, const int& origin, const int& clvl, const std::string& msg)
	: sender(sender)
	, signal_id(signal_id)
	, origin(origin)
	  , clvl(clvl)
	, msg(msg.c_str())
{ }

const Entry& Message::get_sender(){
	return this->sender;
}

const int& Message::get_signal_id(){
	return this->signal_id;
}

const int& Message::get_origin(){
	return this->origin;
}

const int& Message::get_sender_clvl(){
	return this->clvl;
}
