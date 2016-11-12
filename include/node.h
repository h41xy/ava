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
 public:
	int run(char*); // own_id
	std::list<int> get_nb_ids(std::string,int); //gfname, own_id
	int socialise_myself(Addressbook,Entry);//book, myself
	int send_msg_to_all(Addressbook,std::string); //book,msg
	int send_id_to_neighbor(int,std::string,int); //myid,recv_ip,recv_port
	
};
/* The main method to start a single node.
 * It requires one argument which is used as the own ID
 */
int main ( int argc, char *argv[]) {
	if(argc > 1){
		Node node;
		node.run(argv[1]);
	}else
		std::cout << "Please give a ID as Argument.\n";
}
