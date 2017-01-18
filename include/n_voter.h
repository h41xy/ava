#ifndef N_VOTER_H
#define N_VOTER_H

#include "node.h"

#include <map>
#include <random>

class N_voter : public Node {
protected:
	Addressbook candidates;

	int candidate_count;
	std::map<int, int> candidate_c_levels;

	std::list<int> get_candidate_ids(const std::string&);
	int get_random(const int&, int);
	int eliminate_candidates_from_neighbors(Addressbook&, Addressbook&);
	int vote_me_response(Message&);
	int find_id_of_max_value(std::map<int, int>&, int&, bool&);

	int init_as_partybuddy(Message&);
public:
	N_voter(char*);

	int run();
};
#endif // N_VOTER_H
