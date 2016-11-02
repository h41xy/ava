// for testing stuff

#include <iostream>

#include "addressbook.h"

int main(){
	Addressbook book("example.txt"); 
	std::cout << book.entrycount() << "\n";
}
