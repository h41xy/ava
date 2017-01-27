#include "message.h"

Message::Message(const Entry& sender, const int& signal_id, const int& origin, const int& clvl, const std::string& msg)
	: sender(sender)
	, signal_id(signal_id)
	, origin(origin)
	  , clvl(clvl)
{
	std::strcpy(this->msg, msg.c_str());
	msg_id = get_random(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
	std::fill(std::begin(vtimestamp),std::end(vtimestamp),0);
}

Message::Message(const Entry& sender, const int& signal_id, const int& origin, const int& clvl, const std::string& msg, int new_vtimestamp[MAX_VECTORTIME_SIZE])
	: sender(sender)
	, signal_id(signal_id)
	, origin(origin)
	  , clvl(clvl)
{
	std::strcpy(this->msg, msg.c_str());
	msg_id = get_random(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
	set_vtimestamp(new_vtimestamp);
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

int Message::set_msg(std::string new_msg){
	std::strcpy(this->msg, new_msg.c_str());
	return -1;	
}

int Message::set_vtimestamp(std::vector<int> inc_vtimestamp){
	for (unsigned int i = 0; i < inc_vtimestamp.size(); i++){
		vtimestamp[i] = inc_vtimestamp[i];
	}
	return-1;
}

int Message::set_vtimestamp(int inc_vtimestamp[MAX_VECTORTIME_SIZE]){
	// TODO dynamic array size of inc vtimestamp as border, think about new init of all arrays
	for (int i = 0; i < MAX_VECTORTIME_SIZE; i++) {
		vtimestamp[i] = inc_vtimestamp[i];
	}
	return -1;
}
