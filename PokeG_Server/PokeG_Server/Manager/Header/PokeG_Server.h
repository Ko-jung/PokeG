#pragma once

#include "../../stdafx.h"
#include "../../Enum.h"
#include "../../OverExpansion.h"

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

	std::unordered_map<COMP_TYPE, std::function<void(int, int, OverExpansion*)>> CompFuncMap;
	void ProcessAccept(	int id, int bytes, OverExpansion* exp);
	void ProcessRecv(	int id, int bytes, OverExpansion* exp);
	void ProcessSend(	int id, int bytes, OverExpansion* exp);

private:
	std::vector<std::thread> WorkerThreads;
	int WorkerNum;

	HANDLE Iocp;

	class OverExpansion* AcceptExpOver;
};

