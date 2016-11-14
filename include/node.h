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
const int RECV_MSG = 1;
const int SOCIALISE = 2;

const int MSG_BUFFER_SIZE = 256;

class Node{
	Entry myself;
	Addressbook book;
	int myid; //not pretty but I need it before I get myself

 public:
	Node(char*); // own ID
	int run(); // Main loop

	int send_all_signal(int);
	int send_all_msg(std::string);

	std::list<int> get_nb_ids(std::string,int); //gfname, own_id
	
};

/* The main method to start a single node.
 * It requires one argument which is used as the own ID
 */
int main ( int argc, char *argv[]) {
	if(argc > 1){
		Node node(argv[1]);
		node.run();
	}else
		std::cout << "Please give a ID as Argument.\n";
}
