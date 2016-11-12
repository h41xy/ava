// This program createss a random gaph and writes it in a file in the graphviz format

#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char* argv[]){
	std::ostringstream os;
	os << "graph G{\n";
	os << "\n}";
	std::cout << os.str() << std::endl;
}
