#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include "entry.h"

class Message {
protected:
	const Entry sender;
	const int signal_id;
	const int origin;
	const int clvl;
	char msg[MSG_BUFFER_SIZE];
public:
	Message(const Entry&, const int&, const int&, const int&, const std::string&);

	const Entry& get_sender() const;
	const int& get_signal_id() const;
	const int& get_origin() const;
	const int& get_sender_clvl() const;
	const std::string& get_msg() const;
};
#endif // MESSAGE_H
