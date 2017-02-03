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
	
	struct Acknowledge{
		int id;
		int ltimestamp;
		bool acknowledged;
	};

	struct QEntry{
		QEntry(int id, int ltimestamp) : id(id),ltimestamp(ltimestamp) {}
		int id;
		int ltimestamp;
	};

	std::vector<Acknowledge> acknowledges;
	std::queue<QEntry> request_queue;

	int listen_loop(Listener&);
	bool process_recvd_msg(Message&);

	int send_message(Entry&, Message&);
	int send_all_message(Addressbook&, Message&);

	// switch case methods
	int sc_exit_all(Message&);

	int start_request();
	int received_request(int, int); // id, ltime
	int increment_ltime();
	int send_request(int, int);
	int send_ack(int);

	// msg handling
	std::stringstream ss;
	int logger_signal_out(Entry&, Message&, const bool&); // Receiver, Message, connection fail/success
	int logger_signal_in(Message&);

	int clear_stringstream(std::stringstream&);
};
#endif // NODE_H
