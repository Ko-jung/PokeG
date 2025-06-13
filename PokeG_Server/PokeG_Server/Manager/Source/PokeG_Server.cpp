#include "../Header/PokeG_Server.h"
#include "../../Util/UtilLog.h"
#include "../../Enum.h"

#include <iostream>

PokeG_Server::PokeG_Server()
{
	AcceptExpOver = new OverExpansion;
	CompFuncMap			[COMP_TYPE::OP_ACCEPT] = [this](OverExpansion* exp) { this->ProcessAccept(exp); };
	CompFuncMap.insert({ COMP_TYPE::OP_RECV,	std::bind(&PokeG_Server::ProcessRecv, this, std::placeholders::_1) });
	CompFuncMap.insert({ COMP_TYPE::OP_SEND,	std::bind(&PokeG_Server::ProcessSend, this, std::placeholders::_1) });
}

PokeG_Server::~PokeG_Server()
{
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

	*(reinterpret_cast<SOCKET*>(&AcceptExpOver->_send_buf)) = c_socket;
	ZeroMemory(&AcceptExpOver->_over, sizeof(AcceptExpOver->_over));
	AcceptExpOver->_comp_type = COMP_TYPE::OP_ACCEPT;

	AcceptEx(ListenSocket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &AcceptExpOver->_over);
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

		int client_id = static_cast<int>(IocpKey);
		OverExpansion* Exp = reinterpret_cast<OverExpansion*>(p_over);
		
		if (FALSE == ret)
		{
			int err_no = WSAGetLastError();
			UtilLog::Log("GQCS Error : ");
			UtilLog::ErrorDisplay(err_no);
			Disconnect(client_id);
			if (Exp->_comp_type == COMP_TYPE::OP_SEND)
				delete Exp;
			continue;
		}

		if (0 == NumByte)
		{
			if (Exp->_comp_type == COMP_TYPE::OP_SEND || Exp->_comp_type == COMP_TYPE::OP_RECV)
				UtilLog::Log("0 == NumByte");
				//ClientMgr::Instance()->Disconnect(client_id);
		}

		if (CompFuncMap.find(Exp->_comp_type) != CompFuncMap.end())
		{
			CompFuncMap[Exp->_comp_type](Exp);
		}
		else
		{
			assert(false, "Unknown Completion Type");
		}
	}
}

void PokeG_Server::ProcessAccept(OverExpansion* exp)
{
	//if (ClientMgr::Instance()->GetClientCount() < MAX_USER)
	//{
	//	int NowClientNum;
	//	std::shared_ptr<Client> NewClient = ClientMgr::Instance()->GetEmptyClient(NowClientNum);
	//	if (NewClient == nullptr)
	//	{
	//		std::cerr << "Client NULL!" << std::endl;
	//		return;
	//	}
	//
	//	NewClient->ClientNum = NowClientNum;
	//	NewClient->Socket = (*(reinterpret_cast<SOCKET*>(exp->_send_buf)));
	//
	//	CreateIoCompletionPort(reinterpret_cast<HANDLE>(NewClient->Socket), hIocp, NowClientNum, 0);
	//
	//	NewClient->Recv();
	//
	//	ReadyAccept();
	//}
	//else
	//{
	//	std::cerr << "Client MAX!" << std::endl;
	//}
}

void PokeG_Server::ProcessRecv(OverExpansion* exp)
{
}

void PokeG_Server::ProcessSend(OverExpansion* exp)
{
	delete exp;
}

void PokeG_Server::Start()
{
	Init();
	BindListen(9998);
	CreateThread();
	ThreadJoin();
}
