#include "Client.h"
#include "OverExpansion.h"
#include "Packet.h"

#include "Protocol.h"
#include "Util/UtilLog.h"
#include "Manager/Header/PacketMgr.h"

Client::Client()
{
	Exp = new OverExpansion;
	ZeroMemory(&Exp->Over, sizeof(Exp->Over));

	RemainDataLen = 0;
	ClientNum = -1;
	Socket = INVALID_SOCKET;
}

Client::~Client()
{
	delete Exp;
}

void Client::ProcessRecv(int bytes, OverExpansion* exp)
{
	assert(ClientNum >= 0 && ClientNum < MAX_USER);

	int RemainData = bytes + RemainDataLen;
	char* Buf = exp->SendBuf;

	while (RemainData > 0)
	{
		Packet* packet = reinterpret_cast<Packet*>(Buf);
		if (RemainData < packet->Size)
		{
			PacketMgr::Instance()->ProcessPacket(ClientNum, packet);
			Buf += packet->Size;
			RemainData -= packet->Size;
		}
		else
		{
			break;
		}
	}
	RemainDataLen = RemainData;

	if (RemainData > 0)
		memmove(exp->SendBuf, Buf, RemainData);
	Recv();
}

void Client::Recv()
{
	DWORD recv_flag = 0;

	ZeroMemory(&Exp->Over, sizeof(Exp->Over));
	Exp->Wsabuf.buf = reinterpret_cast<char*>(Exp->SendBuf + RemainDataLen);
	Exp->Wsabuf.len = sizeof(Exp->SendBuf) - RemainDataLen;

	int ret = WSARecv(Socket, &Exp->Wsabuf, 1, 0, &recv_flag, &Exp->Over, NULL);
	if (SOCKET_ERROR == ret)
	{
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
		{
			UtilLog::Log("[Recv Error] ClientNum: %d", ClientNum);
		}
	}
}

void Client::Send(Packet* packet)
{
}
