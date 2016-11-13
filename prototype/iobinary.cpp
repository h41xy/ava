// Prototype of binary serialisation
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

class Tasskaff{
 public:
	int coffein;
	//std::string name;
	char name[11];
};
int main(int argc, char*argv[]){

	if(argc ==2){
		Tasskaff eins;
		eins.coffein = 5;
		//eins.name = "blackmetal";
		std::strcpy(eins.name,"blackmetal");

		std::ofstream ofs("prototype/prototype.ofs", std::ios::binary);
		int id = 3333;
		ofs.write(reinterpret_cast<const char *>(&id), sizeof(id));
		ofs.write((char *)&eins, sizeof(eins));
	}else{

		Tasskaff zwo;

		std::ifstream ifs("prototype/prototype.ofs", std::ios::binary);
		int recv_id = 0;
		ifs.read((char *)&recv_id, sizeof(recv_id));
		std::cout << "Recieved ID: " << recv_id << std::endl;
		ifs.read((char *)&zwo, sizeof(zwo));

		std::cout << zwo.coffein << " " << zwo.name << std::endl;
	}

}
