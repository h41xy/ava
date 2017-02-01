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
	int myid; //not pretty but I need it before I get myself

	int listen_loop(Listener&);
	int process_recvd_msg(Message&);

	int send_message(Entry&, Message&);
	int send_all_message(Addressbook&, Message&);

	// switch case methods
	int sc_exit_node(Message&, bool&);
	int sc_exit_all(Message&, bool&);

	// msg handling
	std::stringstream ss;
	int logger_signal_out(Entry&, Message&, const bool&); // Receiver, Message, connection fail/success
	int logger_signal_in(Message&);

	int clear_stringstream(std::stringstream&);
};
#endif // NODE_H
