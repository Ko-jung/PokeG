#pragma once

#include "../../Define.h"
#include "../../stdafx.h"

class PacketMgr
{
	SINGLETON(PacketMgr);

public:
	void ProcessPacket(int ClientId, class Packet* packet);
};

