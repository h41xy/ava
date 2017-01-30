#ifndef WATCHER_H
#define WATCHER_H

#include "node.h"

class Watcher : public Node {
protected:
	int candidates_c_sums[2];
	// increment on sending out, decrement on receiving, if 0 is reached again, terminate
	int responsecounter;
	// no vector because the transmission is easier and the node can fill its own vector with
	// this value
	int vtime_to_respond;

	// send msg to all with vtime on which a response shoul be given
	int request_response_on_vtime(int);

	// process receiving message
	// gather all received responses
	int process_recvd_response(Message&);

	// process the gathered responses
	// 	when responsecounter reaches 0
	int process_all_responses();

	// write the result to file
	int write_to_file(std::string&);

public:
	// Get the char* to WATCHER_ID constant
	Watcher(char*);

	int run();
};
#endif // WATCHER_H
