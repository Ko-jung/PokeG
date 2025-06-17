#include "stdafx.h"
#include "Define.h"

#include "../../Common/protocol.h"

#include "IOCPServer.h"

#define UNICODE

int main()
{
	setlocale(LC_ALL, "");
	std::wcout.imbue(std::locale(""));

	PokeGServer* IocpServer = new PokeGServer();

	IocpServer->Init(std::thread::hardware_concurrency());
	IocpServer->BindListen(GAMESERVERPORT);

	IocpServer->StartServer();

	IocpServer->ThreadJoin();
}