#include "Manager/Header/PokeG_Server.h"

#include <iostream>

int main()
{
	setlocale(LC_ALL, "");
	std::wcout.imbue(std::locale(""));

	// Create a new instance of the server
	PokeG_Server Server;
	// Start the server
	Server.Start();

	return 0;
}