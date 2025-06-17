#include "../Header/ClientMgr.h"

#include "../../Util/UtilLog.h"

ClientMgr::ClientMgr()
{
	ClientCount = 0;

}

ClientMgr::~ClientMgr()
{
}

std::shared_ptr<Client> ClientMgr::GetEmptyClient(int& ClientNum)
{
	for (int i = 0; i < Clients.size(); i++)
	{
		//TODO: °³¼±
		std::lock_guard<std::mutex> gLock(ClientMutex);
		std::shared_ptr<Client> client = Clients[i];
		if (!client)
		{
			client = std::make_shared<Client>();
			ClientCount++;
			ClientNum = i;
			return client;
		}
	}

	return nullptr;
}

void ClientMgr::ProcessRecv(int id, int bytes, OverExpansion* exp)
{
	std::shared_ptr<Client> client = Clients[id];
	if (!client)
	{
		UtilLog::Log("Client not found for ID: {}", id);
		return;
	}

	client->ProcessRecv(bytes, exp);
}

void ClientMgr::ProcessLogin(Packet* packet, const int ClientNum)
{
}
