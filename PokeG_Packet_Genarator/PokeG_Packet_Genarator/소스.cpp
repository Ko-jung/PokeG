#include <iostream>
#include <string>
#include <fstream>

#define CONSTRUCTOR(classname) \
	public:\
	classname()\
	{ \
	} \
	~classname()\
	{ \
	}

int main()
{
	std::fstream file("Packet.txt", std::ios::in);

	if (!file.is_open())
	{
		std::cout << "Error opening file!" << std::endl;
		return 1;
	}

	std::string path = "..//..//PacketClass//";
	//std::ofstream HeaderFile(	path + "ABC" + ".h");
	std::ofstream HeaderFile(	path + "ABC" + ".h");
	std::ofstream CppFile(		path + "ABC" + ".cpp");

	if (!HeaderFile.is_open()) {
		std::cerr << "HeaderFile 파일을 생성할 수 없습니다: " << path << std::endl;
		return 1;
	}
	if (!CppFile.is_open()) {
		std::cerr << "CppFile 파일을 생성할 수 없습니다: " << path << std::endl;
		return 1;
	}

	HeaderFile << "#pragma once\n";
	CppFile << "#include \"ABC.h\"\n\n";

	HeaderFile.close();
	CppFile.close();
}