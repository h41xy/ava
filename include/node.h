// The node
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>
#include <unistd.h>
#include <vector>

#include "constants.h"
#include "addressbook.h"
#include "listener.h"
#include "sender.h"

class Node{
protected:
	Entry myself;
	Addressbook book;
	Addressbook neighbors;
	int myid; //not pretty but I need it before I get myself

	int believe_border, rumor_counter;
	bool heard_rumor, believe_rumor;

	// Vector clock
	std::vector<int> vtime;

	int vtime_up(std::vector<int>&);

	int send_all_signal(Addressbook,int);
	int send_all_msg(Addressbook,std::string);
	int send_all_rumor(Addressbook,int,int);

	// switch case methods
	int sc_exit_node(bool&);
	int sc_exit_all(bool&);
	int sc_recv_msg(int&);
	int sc_socialise();
	int sc_rumor(int&);
	int sc_print_vtime();

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id

	// msg handling
	std::stringstream ss;
	int msg_out(std::list<Entry>::iterator&, const std::string&,const bool&); // iterator for receiver ip and Port, msg
	int msg_out(const std::string&,const int&,const std::string&,const bool&); // ip, port, msg, connection success or failed
	int signal_out(std::list<Entry>::iterator&,int&,const bool&); // iterator for receiverID, signalid, connection success or failed

	int signal_in(const int&); // Signal ID
	int msg_in(const int&, const std::string&);
	int clear_stringstream(std::stringstream&);

 public:
	Node(char*); // own ID
	Node(char*,char*); // own ID, believe_border
	int run(); // Main loop
	
	std::vector<int> get_vectortime();
};
