#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <random>
#include <limits>
#include "entry.h"

class Message {
protected:
	const Entry sender;
	const int signal_id;
	const int ltime;
	unsigned int msg_id;
	char msg[MSG_BUFFER_SIZE];

	unsigned int get_random(const unsigned int&, const unsigned int&); // min max

public:
	Message();
	Message(const int);
	Message(const Entry, const int, std::string);
	Message(const Entry, const int, const int);
	// Sender Entry, Signal ID, Lamport time, Message
	Message(const Entry, const int, const int, std::string);

	const Entry& get_sender() const;
	const int& get_signal_id() const;
	const int& get_ltime() const;
	const unsigned int& get_msg_id() const;
	const std::string get_msg() const;

	int set_msg_id(unsigned int);
	int set_msg(std::string);
	};
#endif // MESSAGE_H
