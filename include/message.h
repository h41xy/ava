#ifndef MESSAGE_H
#define MESSAGE_H

#include "entry.h"

class Message {
protected:
	const Entry sender;
	const int signal_id;
	const int origin;
	const int clvl;
public:
	Message(Entry&, int&, int&, int&);

	const Entry& get_sender();
	const int& get_signal_id();
	const int& get_origin();
	const int& get_sender_clvl();
};
#endif // MESSAGE_H
