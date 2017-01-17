#ifndef N_VOTER_H
#define N_VOTER_H

#include "node.h"

#include <map>
#include <ctime> // for random seed

class N_voter : public Node {
protected:
	Addressbook candidates;

	int candidate_count;
	std::map<int, int> candidate_c_levels;

	std::list<int> get_candidate_ids(const std::string&);
	int get_random(const int&, int);
public:
	N_voter(char*);

	int run();
};
#endif // N_VOTER_H
