// The node
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <cerrno>

#include "addressbook.h"
#include "listener.h"
#include "sender.h"

class Node{
	Entry myself;
	Addressbook book;
	int myid;
 public:
	Node(char*);
	int run();
	std::list<int> get_nb_ids(std::string,int); //gfname, own_id
	int socialise_myself();
	int send_msg_to_all(std::string); //msg
	int send_id_to_neighbor(std::string,int); //recv_ip,recv_port
	
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
