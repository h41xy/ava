#include "node.h"

class N_rumor : public Node {
protected:
	int believe_border, rumor_counter;
	bool heard_rumor, believe_rumor;

	int send_all_rumor(Addressbook,int,int);
	int sc_rumor(int&);
public:
	N_rumor(char*,char*); // own ID, believe_border

	int run(); // Main loop
};
