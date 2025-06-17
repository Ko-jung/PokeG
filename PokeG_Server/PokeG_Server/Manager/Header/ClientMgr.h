#pragma once

#include "../../Define.h"
#include "../../stdafx.h"
#include "../../Protocol.h"

#include "../../Client.h"

class ClientMgr
{
	SINGLETON(ClientMgr);

public:
	int GetClientCount() const { return ClientCount.load(); }
	std::shared_ptr<Client> GetEmptyClient(int& ClientNum);

	void ProcessRecv(int id, int bytes, class OverExpansion* exp);
	void ProcessLogin(class Packet* packet, const int ClientNum);

private:
	ClientMgr();
	~ClientMgr();

	std::array<std::shared_ptr<Client>, MAX_USER + MAX_NPC> Clients;
	std::atomic_int ClientCount;
	std::mutex ClientMutex;
};

