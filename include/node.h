// The node
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>
#include <unistd.h>
#include <vector>
#include <random>

#include "constants.h"
#include "addressbook.h"
#include "listener.h"
#include "sender.h"
#include "message.h"

class Node{
protected:
	Entry myself;
	Addressbook book;
	Addressbook neighbors;
	int myid; //not pretty but I need it before I get myself


	// Vector clock
	std::vector<int> vtime;

	int vtime_up(std::vector<int>&);

	int send_signal(Entry&, const int&);
	int send_all_signal(Addressbook,int);

	int send_message(Entry&, Message&);
	int send_all_message(Addressbook&, Message&);
	// forward is like send but excepts the sender
	// TODO theses are unfinished methods
	int forward_message(Entry&, Message&);
	int forward_all_message(Addressbook&, Message&);

	// ECHO alg
	enum NodeColor { white, red, green };
	NodeColor state;
	int echo_counter;
	Entry first_neighbor;

	// watch out for return values
	unsigned int get_random(const unsigned int&, unsigned int);

	// switch case methods
	int sc_exit_node(Message&, bool&);
	int sc_exit_all(Message&, bool&);
	int sc_print_vtime();

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id

	// msg handling
	std::stringstream ss;
	int logger_signal_out(Entry&, Message&, const bool&); // Receiver, Message, connection fail/success
	int logger_signal_in(Message&);

	int clear_stringstream(std::stringstream&);

	// ECHO Alg
	int process_echo_explore(Message&);
	int process_echo(Message&);

 public:
	Node(char*); // own ID
	int run(); // Main loop
	
	std::vector<int> get_vectortime();
};
#endif // NODE_H
