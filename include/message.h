#ifndef MESSAGE_H
#define MESSAGE_H

#include "entry.h"

class Message {
protected:
	const Entry sender;
	const int signal_id;
	const int origin;
	const int clvl;
	const char msg[256];
public:
	Message(const Entry&, const int&, const int&, const int&, const std::string&);

	const Entry& get_sender();
	const int& get_signal_id();
	const int& get_origin();
	const int& get_sender_clvl();
	const std::string& get_msg();
};
#endif // MESSAGE_H
