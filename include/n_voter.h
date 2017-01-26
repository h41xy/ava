#ifndef N_VOTER_H
#define N_VOTER_H

#include "node.h"

#include <map>

class N_voter : public Node {
protected:
	Addressbook candidates;
	// needed for ECHO alg
	Addressbook neighbors_wo_candidates;

	int candidate_count;
	std::map<int, int> candidate_c_levels;

	std::vector<unsigned int> known_signals;

	std::list<int> get_candidate_ids(const std::string&);
	int eliminate_candidates_from_neighbors(Addressbook&, Addressbook&);
	int vote_me_response(Message&);
	int find_id_of_max_value(std::map<int, int>&, int&, bool&);

	int vtime_check_terminate(std::vector<int>&, std::vector<int>&, bool&); // above three values

	int init_as_partybuddy(Message&);

	// ECHO Alg
	int v_process_echo_explore(Message&);
public:
	N_voter(char*);

	int run();
};
#endif // N_VOTER_H
