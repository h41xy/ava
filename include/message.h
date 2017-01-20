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
	const int origin;
	const int clvl;
	unsigned int msg_id;
	char msg[MSG_BUFFER_SIZE];

	unsigned int get_random(const unsigned int&, const unsigned int&); // min max

	// vector time
	int vtimestamp[MAX_VECTORTIME_SIZE];
public:
	// Myself, signalid, origin, clvl, msg
	Message(const Entry&, const int&, const int&, const int&, const std::string&);

	const Entry& get_sender() const;
	const int& get_signal_id() const;
	const int& get_origin() const;
	const int& get_sender_clvl() const;
	const unsigned int& get_msg_id() const;
	const std::string get_msg() const;

	int set_msg_id(unsigned int);
	int set_vtimestamp(int[MAX_VECTORTIME_SIZE]);
	};
#endif // MESSAGE_H
