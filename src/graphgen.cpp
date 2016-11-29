// This program createss a random gaph and writes it in a file in the graphviz format

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#include "constants.h"

int get_random_node(int range){
	if(range == 0)
		return 1;
	return (std::rand() % range) + 1;
}

int run(char* m_cstr, char* n_cstr){
	std::srand(std::time(0));
	int nodes, edges, candidate, candidate_two, inserted_edges = 0;
	edges = std::stoi(std::string(m_cstr)); // Kantenzahl
	nodes = std::stoi(std::string(n_cstr)); // Knotenzahl

	if(edges < nodes)
		return -1;

	std::ostringstream os;
	os << "graph G{\n";

	bool edge_exists[nodes][nodes];
	for(int i=2;i<=nodes;i++){
		do{
			candidate = get_random_node(i-1);
		}while(candidate == i || edge_exists[candidate][i]);
		edge_exists[candidate][i] = true;
		os << candidate << " -- " << i << ";\n";
		inserted_edges++;
	}
	while(inserted_edges++<edges){
		do{
			candidate = get_random_node(nodes);
			candidate_two = get_random_node(nodes);
		// TODO if more edges than pssible -> deadlock
		}while(candidate == candidate_two || edge_exists[candidate][candidate_two]);
		os << candidate << " -- " << candidate_two << ";\n";
	}

	os << "}";
	//std::cout << os.str() << std::endl;
	std::ofstream ofs;
	ofs.open(GRAPHFILE);
	ofs << os.str();
	ofs.close();
	return 0;
}

int main(int argc, char* argv[]){
	if(argc < 3)
		std::cout << "Usage: Give two arguments m and n, where m > n\n";
	else
		if (run(argv[1],argv[2]) == -1)
			std::cout << "Something went wrong. (m < n?)\n";
		else
			std::cout << "Outputfile generated.\n";
}
