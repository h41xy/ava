// Main for the Node

#include "node.h"


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
