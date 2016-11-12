// This program createss a random gaph and writes it in a file in the graphviz format

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

int get_random_node(int range){
	if(range == 0)
		return 1;
	return (std::rand() % range) + 1;
}

int run(char* m_cstr, char* n_cstr){
	std::srand(std::time(0));
	int nodes, edges, inserted_edges = 0;
	edges = std::stoi(std::string(m_cstr)); // Kantenzahl
	nodes = std::stoi(std::string(n_cstr)); // Knotenzahl

	if(edges < nodes)
		return -1;

	std::ostringstream os;
	os << "graph G{\n";

	for(int i=2;i<=nodes;i++){
		os << get_random_node(i-1) << " -- " << i << ";\n";
		inserted_edges++;
	}
	while(inserted_edges++<edges){
		os << get_random_node(nodes) << " -- " << get_random_node(nodes) << ";\n";
	}

	os << "}";
	//std::cout << os.str() << std::endl;
	std::ofstream ofs;
	ofs.open("doc/genexample_graphviz.txt");
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
