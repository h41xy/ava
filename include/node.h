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
private:
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
	int sc_exit_node(std::time_t&, bool&);
	int sc_exit_all(std::time_t&, bool&);
	int sc_recv_msg(std::time_t&, int&);
	int sc_socialise();
	int sc_rumor(std::time_t&, int&);
	int sc_print_vtime();

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id

	// msg handling
	std::stringstream ss;
	int msg_out(int&,std::list<Entry>::iterator&,std::string&,const bool&); // OwnID, iterator for ip and Port, msg
	int clear_stringstream(std::stringstream&);

 public:
	Node(char*); // own ID
	Node(char*,char*); // own ID, believe_border
	int run(); // Main loop
	
	std::vector<int> get_vectortime();
};
