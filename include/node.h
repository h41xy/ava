// The node
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>
#include <unistd.h>

#include "addressbook.h"
#include "listener.h"
#include "sender.h"

const int EXIT_NODE = 0;
const int EXIT_ALL = 1;
const int RECV_MSG = 2;
const int SOCIALISE = 3;
const int RUMOR = 4;

const int MSG_BUFFER_SIZE = 256;

class Node{
	Entry myself;
	Addressbook book;
	Addressbook neighbors;
	int myid; //not pretty but I need it before I get myself

	int believe_border, rumor_counter;
	bool heard_rumor, believe_rumor;

	int send_all_signal(Addressbook,int);
	int send_all_msg(Addressbook,std::string);

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id

 public:
	Node(char*); // own ID
	Node(char*,char*); // own ID, believe_border
	int run(); // Main loop
};

/* The main method to start a single node.
 * It requires one argument which is used as the own ID
 */
int main ( int argc, char *argv[]) {
	if(argc == 2){
		Node node(argv[1]);
		node.run();
	}else if(argc == 3){
		Node node(argv[1], argv[2]);
		node.run();
	}else
		std::cout << "Please give a ID as Argument.\n";
}
