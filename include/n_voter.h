#ifndef N_VOTER_H
#define N_VOTER_H

#include "node.h"

class N_voter : public Node {
protected:
	Addressbook candidates;

	int c_level;

	std::list<int> get_candidate_ids(const std::string&);
public:
	N_voter(char*);

	int run();
};
#endif // N_VOTER_H
