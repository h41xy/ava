// This program createss a random gaph and writes it in a file in the graphviz format

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

int get_random_node(int nodecount){
	std::srand(std::time(0));
	return (std::rand() % nodecount) + 1;
}

int run(char* m_cstr, char* n_cstr){
	int m,n;
	m = std::stoi(std::string(m_cstr)); // Kantenzahl
	n = std::stoi(std::string(n_cstr)); // Knotenzahl

	if(m < n)
		return -1;

	std::ostringstream os;
	os << "graph G{\n";
	os << m << " " << n << " " << get_random_node(n);
	os << "\n}";
	std::cout << os.str() << std::endl;
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
