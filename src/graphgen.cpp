// This program createss a random gaph and writes it in a file in the graphviz format

#include <iostream>
#include <string>
#include <sstream>

int run(char* m_cstr, char* n_cstr){
	int m,n;
	m = std::stoi(std::string(m_cstr));
	n = std::stoi(std::string(n_cstr));

	if(m < n)
		return -1;

	std::ostringstream os;
	os << "graph G{\n";
	os << m << " " << n;
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
