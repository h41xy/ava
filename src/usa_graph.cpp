// TODO if voterfreindcount is too high it becomes an endlessloop
// TODO iostream not needed if print array gets removed
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

#include "constants.h"

// Print given array with size to std out
void print_array(int **m, int& size){
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			std::cout << m[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

// returns a random value in the range starting at 1
// zero is never returned
int get_random_node(const int& max, const int& min){
	if(max == 0)
		return 1;
	return (std::rand() % max) + min;
}

// Prints a given 2D array to the file GRAPHFILE in graphviz format
int print_matrix_to_graphviz(int **m, int& size, int& candidates){

	// Prepare stringstream
	std::ostringstream os;
	os << "graph G{\n";

	// Print the candidates with their connections to the partybuddies
	for (int j = 0; j < candidates; j++){
		for (int i = 0; i < size; i++){
			if (m[j][i] == 1)
				os << "c" << j + 1 << " -- " << i - candidates + 1 << "\n";
		}
	}

	// Print the voters from which some are partybuddies
	// The p offset secures only half the matrix is printed and the connections which are bidirectional 
	// doesn't haveto get double checked
	int p = 0;
	for(int j = candidates; j < size; j++){
		for(int i = size; i > candidates - 1 + p; i--){
			if (m[j][i] == 1)
				os << j - candidates + 1 << " -- " << i - candidates + 1 << "\n";
		}
		p++;
	}


	// Finish the outputstream and print it to the file
	os << "}";
	std::ofstream ofs;
	ofs.open(GRAPHFILE);
	ofs << os.str();
	ofs.close();

	return -1;
}

// Creates a graph with <nocecount> nodes from whom <candidatecount> are candidates.
// These candidates only connect to <partybuddiecount> partybuddies from whom each candidate has equal
// All n - c leftover nodes are voters and are in a connected random graph
// TODO
// every voter has <voterfriendcount> neighbor nodes to whom he is connected
int run(int& nodecount, int& candidatecount, int& partybuddiecount, int& voterfriendcount){

	// init random
	std::srand(std::time(0));

	// int n // Nodecount
	// int c // Candidates // c <= n
	// int pb // PartyBuddies // n - c - (c * pb) > 0
	// int vf // VoterFriendCount // vf >= n - c && vf < ((n-c)*(n-c-1)/2)

	int people = nodecount;

	// if there are more candidates than nodes, exit
	if ( candidatecount >= nodecount)
		return -1;
	int candidates = candidatecount;

	// if the number of partybuddies for each candidate exceeds teh nodecount, exit
	if ( nodecount - candidatecount - (candidatecount * partybuddiecount) < 0 )
		return -1;
	int party_buddies = partybuddiecount;

	// Voterfreindcount must be at least 1
	if (voterfriendcount == 0)
		return -1;
	int voter_friends = voterfriendcount;

	int free_voters = people - candidates - candidates * party_buddies;

	int ncandidate = 0;

	// Use a new operator so i can pass the array to functions
	int **m;
	m = new int *[18];
	for(int i = 0; i < 18; i++)
		m[i] = new int[18];

	// init array
	for ( int i = 0; i < people; i++){
		for ( int j = 0; j < people; j++){
			m[i][j] = 0;
		}
	}

	// TODO The offset clusterfuck is real, find something easier to read
	// set candidate buddies

	for (int j = 0; j < candidates; j++){
		for ( int i = candidates + j * party_buddies; i < people - free_voters - ((candidates - (j+1)) * party_buddies) ; i++ ){
			m[j][i] = 1;
			m[i][j] = 1;
		}
	}

	// Construct a connected graph for all Nodes except the candidates
	for (int i = candidates + 1; i < people ; i++) {
		do{
			ncandidate = get_random_node(i-1, 2);
		}while(ncandidate == i || m[ncandidate][i] == 1 || m[i][ncandidate] == 1);

		m[ncandidate][i] = 1;
		m[i][ncandidate] = 1;
	}

	// insert the ordered number of friends (neighbors) per voternode
	for (int j = candidates; j < people; j++) {
		int friends = 0;
		for (int i = candidates; i < people; i++) {
			if ( m[j][i] == 1 )
				friends++;
		}
		while ( friends < voter_friends ) {
			int possible_neighbor = get_random_node(people - candidates - 1, candidates + 1);
			if ( m[j][possible_neighbor] != 1 && m[possible_neighbor][j] != 1 && possible_neighbor != j){
				m[j][possible_neighbor] = 1;
				friends++;
			}
		}
	}

	print_array(m, people);

	print_matrix_to_graphviz(m, people, candidates);

	// dealloc array
	for (int i = 0; i < people; i++)
		delete[] m[i];
	delete[] m;

	return 0;
}

int main(int argc, char* argv[]){

	if ( argc != 5 ){
		std::cout << "Usage: graph <Nodecount> <candidatecount> <partybuddiecount> <voterfriendcount>\n";


	} else {

		// int n // Nodecount
		// int c // Candidates // c <= n
		// int pb // PartyBuddies // n - c - (c * pb) > 0
		// int vf // VoterFriendCount // vf >= n - c && vf < ((n-c)*(n-c-1)/2)
		int nodecount, candidatecount, partybuddiecount, voterfriendcount;
		nodecount = std::stoi(std::string(argv[1]));
		candidatecount = std::stoi(std::string(argv[2]));
		partybuddiecount = std::stoi(std::string(argv[3]));
		voterfriendcount = std::stoi(std::string(argv[4]));

		if (run(nodecount, candidatecount, partybuddiecount, voterfriendcount) == -1)
			std::cout << "Something went wrong. Exited\n";
		else
			std::cout << "Outputfile " << GRAPHFILE << " generated.\n";
		return 0;
	}

	return -1;
}
