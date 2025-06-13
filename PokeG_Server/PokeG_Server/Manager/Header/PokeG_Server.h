#pragma once

#include "../../stdafx.h"
#include "../../Enum.h"
#include "../../OverExpasion.h"

#include <unordered_map>
#include <functional>

class PokeG_Server
{
	SOCKET ListenSocket;
public:
	PokeG_Server();
	~PokeG_Server();

	void Start();
	bool Init();
	bool BindListen(const int PortNum);
	void CreateThread();

	void ReadyAccept();
	void Disconnect(int Id);
	void ThreadJoin();

	void Worker();

	std::unordered_map<COMP_TYPE, std::function<void(OverExpansion*)>> CompFuncMap;
	void ProcessAccept(OverExpansion* exp);
	void ProcessRecv(OverExpansion* exp);
	void ProcessSend(OverExpansion* exp);

private:
	std::vector<std::thread> WorkerThreads;
	int WorkerNum;

	HANDLE Iocp;

	class OverExpansion* AcceptExpOver;
};

