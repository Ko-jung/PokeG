#include "IOCPServer.h"
#include "Define.h"
#include "../../Common/EnumDef.h"
#include "../../Common/protocol.h"
#include "../../Common/OverExpansion.h"

#include "Util/LogUtil.h"

#include "Manager/ClientMgr.h"
#include "Manager/MapMgr.h"
#include "Manager/SectorMgr.h"
#include "Manager/TimerMgr.h"
#include "Manager/DBMgr.h"

#include "Client.h"

PokeGServer::PokeGServer()
{
	AcceptExpOver = new OverExpansion;

	CompFuncMap[COMP_TYPE::OP_ACCEPT] = [this](int id, int byte, OverExpansion* exp) { this->ProcessAccept(id, byte, exp); };
	CompFuncMap.insert({ COMP_TYPE::OP_RECV,	std::bind(&PokeGServer::ProcessRecv, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });
	CompFuncMap.insert({ COMP_TYPE::OP_SEND,	std::bind(&PokeGServer::ProcessSend, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });
	
	CompFuncMap.insert({ COMP_TYPE::OP_NPC_MOVE,		std::bind(&PokeGServer::ProcessNPCMove, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });
	CompFuncMap.insert({ COMP_TYPE::OP_SPAWN_PLAYER,	std::bind(&PokeGServer::ProcessClientSpawn, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3) });

}

PokeGServer::~PokeGServer()
{
}

bool PokeGServer::Init(const int WNum)
{
	// MapMgr Init
	MapMgr::Instance()->Init();
	
	DBMgr::Instance();

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;

	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == ListenSocket)
	{
		LogUtil::ErrorDisplay(WSAGetLastError());
		return false;
	}

	WorkerNum = WNum - 2;
	WorkerThreads.reserve(WorkerNum);

	return true;
}

bool PokeGServer::BindListen(const int PortNum)
{
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PortNum);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(ListenSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (0 != retval)
	{
		LogUtil::ErrorDisplay(WSAGetLastError());
		return false;
	}

	retval = listen(ListenSocket, SOMAXCONN);
	if (0 != retval)
	{
		LogUtil::ErrorDisplay(WSAGetLastError());
		return false;
	}

	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(ListenSocket), hIocp, 9999, 0);

	ReadyAccept();
	std::cout << "Aceept Called\n";

	return true;
}

void PokeGServer::StartServer()
{
	ClientMgr::Instance()->InitNPC();
	TimerMgr::Instance()->SetIOCP(&hIocp);

	for (int i = 0; i < WorkerNum; i++)
	{
		WorkerThreads.emplace_back([this]() { Worker(); });
	}
	//TimerThread = std::thread{ &PokeGServer::Timer, this };
}

void PokeGServer::Worker()
{
	while (true)
	{
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;

		BOOL ret = GetQueuedCompletionStatus(hIocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);

		int client_id = static_cast<int>(iocp_key);
		OverExpansion* Exp = reinterpret_cast<OverExpansion*>(p_over);

		if (FALSE == ret)
		{
			int err_no = WSAGetLastError();
			LogUtil::Log("GQCS Error : ");
			LogUtil::ErrorDisplay(err_no);
			Disconnect(client_id);
			if (Exp->_comp_type == COMP_TYPE::OP_SEND)
				delete Exp;
			continue;
		}

		if (0 == num_byte)
		{
			if (Exp->_comp_type == COMP_TYPE::OP_SEND || Exp->_comp_type == COMP_TYPE::OP_RECV)
			 	ClientMgr::Instance()->Disconnect(client_id);
		}

		//assert(COMP_TYPE::OP_RECV <= Exp->_comp_type && Exp->_comp_type <= COMP_TYPE::OP_SPAWN_PLAYER
		//	&& 0 <= Exp->_wsabuf.len && Exp->_wsabuf.len < 30000);

		switch (Exp->_comp_type)
		{
		case COMP_TYPE::OP_ACCEPT:
			ProcessAccept(client_id, num_byte, Exp);
			break;
		case COMP_TYPE::OP_RECV:
			ProcessRecv(client_id, num_byte, Exp);
			break;
		case COMP_TYPE::OP_SEND:
			ProcessSend(client_id, num_byte, Exp);
			break;
		case COMP_TYPE::OP_NPC_MOVE:
			//ProcessNPCMove(client_id, num_byte, Exp);
			break;
		case COMP_TYPE::OP_SPAWN_PLAYER:
			//ProcessClientSpawn(client_id, num_byte, Exp);
			break;
		default:
			assert(false);
			break;
		}
		//{
		//	std::shared_lock<std::shared_mutex> lock(MapMutex);
		//	if (CompFuncMap.find(Exp->_comp_type) != CompFuncMap.end())
		//	{
		//		assert(Exp);
		//		CompFuncMap[Exp->_comp_type](client_id, num_byte, Exp);
		//	}
		//	else
		//	{
		//		assert(false);
		//	}
		//}
	}
}

void PokeGServer::Timer()
{
	//TimerMgr* TimerInstance = TimerMgr::Instance();
	//while (true)
	//{
	//	TimerInstance->Pop();
	//}
}

void PokeGServer::ThreadJoin()
{
	for (auto& t : WorkerThreads)
	{
		t.join();
	}
}

void PokeGServer::Disconnect(int Id)
{
	ClientMgr::Instance()->Disconnect(Id);
}

void PokeGServer::ReadyAccept()
{
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];

	*(reinterpret_cast<SOCKET*>(&AcceptExpOver->_send_buf)) = c_socket;
	ZeroMemory(&AcceptExpOver->_over, sizeof(AcceptExpOver->_over));
	AcceptExpOver->_comp_type = COMP_TYPE::OP_ACCEPT;

	AcceptEx(ListenSocket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &AcceptExpOver->_over);
}

void PokeGServer::ProcessAccept(int id, int byte, OverExpansion* exp)
{
	if (ClientMgr::Instance()->GetClientCount() < MAX_USER)
	{
		int NowClientNum;
		std::shared_ptr<Client> NewClient = ClientMgr::Instance()->GetEmptyClient(NowClientNum);
		if (NewClient == nullptr)
		{
			std::cerr << "Client NULL!" << std::endl;
			return;
		}

		NewClient->ClientNum = NowClientNum;
		NewClient->Socket = (*(reinterpret_cast<SOCKET*>(exp->_send_buf)));

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(NewClient->Socket), hIocp, NowClientNum, 0);

	 	NewClient->Recv();

		ReadyAccept();
	}
	else
	{
	 	std::cerr << "Client MAX!" << std::endl;
	}
}

void PokeGServer::ProcessRecv(int id, int byte, OverExpansion* exp)
{
	ClientMgr::Instance()->RecvProcess(id, byte, exp);
}

void PokeGServer::ProcessSend(int id, int byte, OverExpansion* exp)
{
	delete exp;
}

void PokeGServer::ProcessNPCMove(int id, int byte, OverExpansion* exp)
{
	ClientMgr::Instance()->ProcessNPCMove(id, exp);
	//delete exp;
}

void PokeGServer::ProcessClientSpawn(int id, int byte, OverExpansion* exp)
{
	ClientMgr::Instance()->ProcessClientSpawn(id);
	//delete exp;
}
 