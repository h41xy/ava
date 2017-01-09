#include <iostream>

void print_array(int m[18][18], int& max){
	for (int i = 0; i < max; i++){
		for (int j = 0; j < max; j++){
			std::cout << m[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

int main (){
	int people = 18;

	int candidates = 2;
	int party_buddies = 5;

	int voter_friends = 3;

	int free_voters = people - candidates - 2 * party_buddies;

	int m[18][18];

	// init array
	for ( int i = 0; i < people; i++){
		for ( int j = 0; j < people; j++){
			m[i][j] = 0;
		}
	}

	print_array(m, people);

	// set candidate buddies
	// Candidate 0
	for ( int i = candidates; i < people - free_voters - party_buddies ; i++ ){
		m[0][i] = 1;
		m[i][0] = 1;
	}

	// Candidate 1
	for (int i = candidates + party_buddies; i < people - free_voters; i ++){
		m[1][i] = 1;
		m[i][1] = 1;
	}
	print_array(m, people);

	// set voter friends
	// std::rand() % 15 + 2;
	// restgraph muss vollstaendig werden also genau wie in graphgen verfahren.
	return -1;
}
