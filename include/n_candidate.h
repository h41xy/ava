#ifndef N_CANDIDATE_H
#define N_CANDIDATE_H

#include "node.h"

class N_candidate : public Node {
protected:
	int response_count;
	int response_border;

	int vote_me();
	int campaign();

	int sc_keep_on();
	int sc_not_you();
	int recv_response();

	// init the partybuddies is hard via script so the candidate does it
	int init_partybuddies();

public:
	N_candidate(char*,char*);	

	// main loop switch cases
	int run();
};
#endif // N_CANDIDATE_H
