#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "constants.h"

void print_array(int **m, int& max){
	for (int i = 0; i < max; i++){
		for (int j = 0; j < max; j++){
			std::cout << m[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

int get_random_node(int range){
	if(range == 0)
		return 1;
	return (std::rand() % range) + 1;
}

int print_matrix_to_graphviz(int **m, int& max, int& candidates){
	std::ostringstream os;
	os << "graph G{\n";

	for (int j = 0; j < candidates; j++){
		for (int i = 0; i < max; i++){
			if (m[j][i] == 1)
				os << "c" << j + 1 << " -- " << i - candidates + 1 << "\n";
		}
	}

	int p = 0;
	for(int j = candidates; j < max; j++){
		for(int i = max; i > candidates - 1 + p; i--){
			if (m[j][i] == 1)
				os << j - candidates + 1 << " -- " << i - candidates + 1 << "\n";
		}
		p++;
	}

	for (int i = candidates; i < max; i++){
		for (int j = candidates; j < max; j++){
		}
	}

	os << "}";
	std::ofstream ofs;
	ofs.open(GRAPHFILE);
	ofs << os.str();
	ofs.close();

	return -1;
}

int run(){
	int people = 18;

	int candidates = 2;
	int party_buddies = 3;

	int voter_friends = 3;

	int free_voters = people - candidates - candidates * party_buddies;

	int ncandidate = 0;

	//int m[18][18];
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

	print_array(m, people);

	// set candidate buddies
	// Candidate 0

for (int j = 0; j < candidates; j++){
	for ( int i = candidates + j * party_buddies; i < people - free_voters - ((candidates - (j+1)) * party_buddies) ; i++ ){
		m[j][i] = 1;
		m[i][j] = 1;
	}
}
//	// Candidate 1
//	for (int i = candidates + party_buddies; i < people - free_voters; i ++){
//		m[1][i] = 1;
//		m[i][1] = 1;
//	}
	print_array(m, people);

	// Construct a connected graph for all Nodes except the candidates
	for (int i = candidates; i < people ; i++) {
		do{
			ncandidate = get_random_node(i-1) + 2;
			std::cout << "Check " << ncandidate << " and " << i << "\n";
		}while(ncandidate == i || m[ncandidate][i] == 1 || m[i][ncandidate] == 1);

		std::cout << "Set " << ncandidate << " and " << i << "\n";
		
		m[ncandidate][i] = 1;
		m[i][ncandidate] = 1;
	}

	print_array(m, people);

	print_matrix_to_graphviz(m, people, candidates);

	// dealloc array
	for (int i = 0; i < people; i++)
		delete[] m[i];
	delete[] m;

	return -1;
}

int main(){

	// int n // Nodecount
	// int c // Candidates // c <= n
	// int pb // PartyBuddies // n - c - (c * pb) > 0
	// int vf // VoterFriendCount // vf >= n - c && vf < ((n-c)*(n-c-1)/2)

	return -1;
}
