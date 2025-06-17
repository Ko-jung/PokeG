#include "../Header/PacketMgr.h"

#include "../../Packet.h"
#include "../../Client.h"

#include "../../Util/UtilLog.h"

void PacketMgr::ProcessPacket(int ClientId, Packet* packet)
{
	assert(ClientId >= 0 && ClientId < MAX_USER);
	assert(packet);

	switch (packet->Type)
	{
	case CS_LOGIN:
	{
		CS_LOGIN_PACKET* loginPacket = static_cast<CS_LOGIN_PACKET*>(packet);

		UtilLog::Log("Client {} logged in with name: {}", ClientId, loginPacket->name);
		break;
	}
	case CS_MOVE:
	{
		CS_MOVE_PACKET* loginPacket = static_cast<CS_MOVE_PACKET*>(packet);

		UtilLog::Log("Client {} logged in with name: {}", ClientId, loginPacket->name);
		break;
	}
	}
}
