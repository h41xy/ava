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
	Entry myself;
	Addressbook book;
	Addressbook neighbors;
	int myid; //not pretty but I need it before I get myself

	int believe_border, rumor_counter;
	bool heard_rumor, believe_rumor;

	// Vector clock
	std::vector<int> vtime;

	int send_all_signal(Addressbook,int);
	int send_all_msg(Addressbook,std::string);
	int send_all_rumor(Addressbook,int,int);

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id

 public:
	Node(char*); // own ID
	Node(char*,char*); // own ID, believe_border
	int run(); // Main loop
	
	std::vector<int> get_vectortime();
};
