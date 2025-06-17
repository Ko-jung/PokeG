#include "../Header/PokeG_Server.h"
#include "../../Util/UtilLog.h"
#include "../../Enum.h"

#include <iostream>

#include "../../Manager/Header/ClientMgr.h"

PokeG_Server::PokeG_Server()
{
	AcceptExpOver = new OverExpansion;
	Iocp = NULL;
	ListenSocket = INVALID_SOCKET;
	WorkerNum = 0;

	CompFuncMap			[COMP_TYPE::OP_ACCEPT] = [this](int id, int byte, OverExpansion* exp) { this->ProcessAccept(id, byte, exp); };
	CompFuncMap.insert({ COMP_TYPE::OP_RECV,	std::bind(&PokeG_Server::ProcessRecv, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });
	CompFuncMap.insert({ COMP_TYPE::OP_SEND,	std::bind(&PokeG_Server::ProcessSend, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });
}

PokeG_Server::~PokeG_Server()
{
	if (AcceptExpOver)
	{
		delete AcceptExpOver;
		AcceptExpOver = nullptr;
	}
	if (ListenSocket != INVALID_SOCKET)
	{
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
	}
	if (Iocp)
	{
		CloseHandle(Iocp);
		Iocp = NULL;
	}
	WSACleanup();

}

bool PokeG_Server::Init()
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;

	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == ListenSocket)
	{
		UtilLog::ErrorDisplay(WSAGetLastError());
		return false;
	}
	 
	WorkerNum = std::thread::hardware_concurrency();
	WorkerThreads.reserve(WorkerNum);

	return true;
}

bool PokeG_Server::BindListen(const int PortNum)
{
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PortNum);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(ListenSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (0 != retval)
	{
		UtilLog::ErrorDisplay(WSAGetLastError());
		return false;
	}

	retval = listen(ListenSocket, SOMAXCONN);
	if (0 != retval)
	{
		UtilLog::ErrorDisplay(WSAGetLastError());
		return false;
	}

	Iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(ListenSocket), Iocp, 9999, 0);

	ReadyAccept();
	UtilLog::Log("Aceept Called");

	return true;
}

void PokeG_Server::CreateThread()
{
	for (int i = 0; i < WorkerNum; i++)
	{
		//WorkerThreads.emplace_back([this]() { Worker(); });
		WorkerThreads.emplace_back(std::bind(&PokeG_Server::Worker,this));
	}
}

void PokeG_Server::ReadyAccept()
{
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];

	*(reinterpret_cast<SOCKET*>(&AcceptExpOver->SendBuf)) = c_socket;
	ZeroMemory(&AcceptExpOver->Over, sizeof(AcceptExpOver->Over));
	AcceptExpOver->CompType = COMP_TYPE::OP_ACCEPT;

	AcceptEx(ListenSocket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &AcceptExpOver->Over);
}

void PokeG_Server::Disconnect(int Id)
{
}

void PokeG_Server::ThreadJoin()
{
	for (auto& t : WorkerThreads)
	{
		t.join();
	}
}

void PokeG_Server::Worker()
{
	while (true)
	{
		DWORD NumByte;
		LONG64 IocpKey;
		WSAOVERLAPPED* p_over;

		BOOL ret = GetQueuedCompletionStatus(Iocp, &NumByte, (PULONG_PTR)&IocpKey, &p_over, INFINITE);

		int ClientId = static_cast<int>(IocpKey);
		OverExpansion* Exp = reinterpret_cast<OverExpansion*>(p_over);
		
		if (FALSE == ret)
		{
			int err_no = WSAGetLastError();
			UtilLog::Log("GQCS Error : ");
			UtilLog::ErrorDisplay(err_no);
			Disconnect(ClientId);
			if (Exp->CompType == COMP_TYPE::OP_SEND)
				delete Exp;
			continue;
		}

		if (0 == NumByte)
		{
			if (Exp->CompType == COMP_TYPE::OP_SEND || Exp->CompType == COMP_TYPE::OP_RECV)
				UtilLog::Log("0 == NumByte");
				//ClientMgr::Instance()->Disconnect(ClientId);
		}

		if (CompFuncMap.find(Exp->CompType) != CompFuncMap.end())
		{
			CompFuncMap[Exp->CompType](ClientId, NumByte, Exp);
		}
		else
		{
			assert(false);
		}
	}
}

void PokeG_Server::ProcessAccept(int id, int bytes, OverExpansion* exp)
{
	if (ClientMgr::Instance()->GetClientCount() < MAX_USER)
	{
		int NowClientNum;
		std::shared_ptr<Client> NewClient = ClientMgr::Instance()->GetEmptyClient(NowClientNum);
		if (NewClient == nullptr)
		{
			UtilLog::Log("Client NULL!");
			//std::cerr << "Client NULL!" << std::endl;
			return;
		}
	
		NewClient->ClientNum = NowClientNum;
		NewClient->Socket = (*(reinterpret_cast<SOCKET*>(exp->SendBuf)));
	
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(NewClient->Socket), Iocp, NowClientNum, 0);
	
		NewClient->Recv();
	
		ReadyAccept();

		UtilLog::Log("Client {} Attecpt", NowClientNum);
	}
	else
	{
		UtilLog::Log("Client MAX!");
		//std::cerr << "Client MAX!" << std::endl;
	}
}

void PokeG_Server::ProcessRecv(int id, int bytes, OverExpansion* exp)
{
	ClientMgr::Instance()->ProcessRecv(id, bytes, exp);
	delete exp;
}

void PokeG_Server::ProcessSend(int id, int bytes, OverExpansion* exp)
{
	delete exp;
}

void PokeG_Server::Start()
{
	Init();
	BindListen(9000);
	CreateThread();
	ThreadJoin();
}
