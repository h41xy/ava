// The node
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cerrno>
#include <unistd.h>
#include <vector>
#include <queue>
#include <map>

#include "constants.h"
#include "addressbook.h"
#include "listener.h"
#include "sender.h"
#include "message.h"

class Node{
public:
	Node(int); // own ID
	int run(); // Main loop

protected:
	Entry myself;
	Addressbook book;
	int id; //not pretty but I need it before I get myself

	int ltime;
	int term_counter;
	
	struct Acknowledge{
		Acknowledge() : Acknowledge(0,0) {}
		Acknowledge(int id, int ltimestamp)
			: id(id)
			, ltimestamp(ltimestamp) {}
		int id;
		int ltimestamp;
	};

	struct QEntry{
		QEntry(int id, int ltimestamp) : id(id),ltimestamp(ltimestamp) {}
		int id;
		int ltimestamp;

		bool operator<(const QEntry& rhs) const{
			// return value inverted so priority queue will have
			// the min item on top

			// second comparsion is id
			if (this->ltimestamp == rhs.ltimestamp)
				return this->id > rhs.id;

			return this->ltimestamp > rhs.ltimestamp;
		}
	};

	std::map<int, Acknowledge> acknowledges;
	std::priority_queue<QEntry> request_queue;

	int listen_loop(Listener&);
	bool process_recvd_msg(Message&);

	int send_message(Entry&, Message&);
	int send_all_message(Addressbook&, Message&);

	// switch case methods
	int sc_exit_all(Message&);

	int start_request();
	int received_request(int, int); // id, ltime
	int received_release(int);
	int received_acknowledge(int, int);

	int exit_cs();
	int enter_cs();
	bool check_access_cs();
	int increment_ltime();
	int increment_ltime(int);
	int send_request(int, int);
	int send_ack(int);
	int send_release(int);

	int cs_processing();

	// msg handling
	std::stringstream ss;
	int logger_signal_out(Entry&, Message&, const bool&); // Receiver, Message, connection fail/success
	int logger_signal_in(Message&);
	int logger_debug_msg(std::string);

	int clear_stringstream(std::stringstream&);
};
#endif // NODE_H
