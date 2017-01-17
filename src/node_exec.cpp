// Main for the Node

#include "node.h"
#include "n_candidate.h"


/* The main method to start a single node.
 */
int main ( int argc, char *argv[]) {
	if(argc == 2){
		Node node(argv[1]);
		node.run();
	}else if(argc == 3){
		N_candidate candidate(argv[1], argv[2]);
		candidate.run();
	}
}
