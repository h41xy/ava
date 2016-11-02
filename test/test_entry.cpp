// for testing stuff

#include <iostream>

#include "entry.h"

int main(){
	Entry entry(1,"127.0.0.1",2500);
	std::cout << entry.getid() << entry.getip() << entry.getport();
}
