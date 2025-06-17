#pragma once
#include "stdafx.h"
#include "../../Common/EnumDef.h"

class PokeGServer
{
public:
	PokeGServer();
	virtual ~PokeGServer();

	bool Init(const int);
	bool BindListen(const int);

	void StartServer();
	void Worker();
	void Timer();
	void ThreadJoin();
	void Disconnect(int Id);

	void ReadyAccept();

	void ProcessAccept(int id, int byte, class OverExpansion* exp);
	void ProcessRecv(int id, int byte, class OverExpansion* exp);
	void ProcessSend(int id, int byte, class OverExpansion* exp);
	void ProcessNPCMove(int id, int byte, class OverExpansion* exp);
	void ProcessClientSpawn(int id, int byte, class OverExpansion* exp);

//protected:
	SOCKET ListenSocket;
	HANDLE hIocp;
	class OverExpansion* AcceptExpOver;

	std::vector<std::thread> WorkerThreads;
	std::thread TimerThread;

	int WorkerNum;

private:
	std::unordered_map<COMP_TYPE, std::function<void(int, int, OverExpansion*)>> CompFuncMap;
};

