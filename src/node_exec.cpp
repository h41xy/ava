// Main for the Node

#include "node.h"
#include <iostream>


/* The main method to start a single node.
 */
int main ( int argc, char *argv[]) {
	if(argc == 2){
		std::string id_str(argv[1]);
		int id = std::stoi(id_str);
		Node node(id);
		node.run();
	}
}
