#include "message.h"

Message::Message(Entry& sender, int& signal_id, int& origin, int& clvl)
	: sender(sender)
	, signal_id(signal_id)
	, origin(origin)
	  , clvl(clvl)
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
