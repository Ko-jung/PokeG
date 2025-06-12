#include "../Header/PokeG_Server.h"
#include "../../UtilLog.h"
#include "../../stdafx.h"
#include <WinSock2.h>

PokeG_Server::PokeG_Server()
{
}

PokeG_Server::~PokeG_Server()
{
}

bool PokeG_Server::Init()
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;

	ListenSocket = std::make_shared<SOCKET>();
	*ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == ListenSocket)
	{
		UtilLog::ErrorDisplay(WSAGetLastError());
		return false;
	}

	WorkerNum = std::thread::hardware_concurrency();
	WorkerThreads.reserve(WorkerNum);

	return true;
}

void PokeG_Server::Start()
{
	Init();
}
