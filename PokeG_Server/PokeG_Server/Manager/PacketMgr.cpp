#include "PacketMgr.h"
#include "ClientMgr.h"
//#include "TimerMgr.h"
//#include "RoomMgr.h"

#include "../Client.h"
//#include "../TimerEvent.h"
#include "../Util/LogUtil.h"

PacketMgr::PacketMgr()
{
	assert(!SingleTonFlag);
	SingleTonFlag = true;
}

PacketMgr::~PacketMgr()
{
}

void PacketMgr::ProcessPacket(PACKET* packet, std::shared_ptr<Client> c)
{
	assert(c->ClientNum >= 0 && c->ClientNum < MAX_USER);
	assert(packet);

	switch (packet->type)
	{
	case CS_LOGIN:
	{
		CS_LOGIN_PACKET* loginPacket = static_cast<CS_LOGIN_PACKET*>(packet);
		ClientMgr::Instance()->ProcessLogin(loginPacket, c);
		//LogUtil::Log("Client {} logged in with name: {}", c->ClientNum, loginPacket->name);
		break;
	}
	case CS_MOVE:
	{
		//ClientMgr::Instance()->ProcessStressTestMove(reinterpret_cast<CS_MOVE_PACKET*>(packet), c);
		break;
	}
	case CS_8DIRECT_MOVE:
	{
		//ClientMgr::Instance()->ProcessMove(reinterpret_cast<CS_8DIRECT_MOVE_PACKET*>(packet), c);
		break;
	}
	case CS_CHAT:
	{
		//ClientMgr::Instance()->ProcessChat(reinterpret_cast<CS_CHAT_PACKET*>(packet), c);
		break;
	}
	case CS_ATTACK:
	{
		//ClientMgr::Instance()->ProcessAttack(reinterpret_cast<CS_ATTACK_PACKET*>(packet), c);
		break;
	}
	case CS_TELEPORT:
	{
		break;
	}
	case CS_LOGOUT:
	{
		break;
	}
	case CS_STATE_CHANGE:
	{
		//ClientMgr::Instance()->ProcessStateChange(reinterpret_cast<CS_STATE_CHANGE_PACKET*>(packet), c);
		break;
	}
	default:
		break;
	}
}