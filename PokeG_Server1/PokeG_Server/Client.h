#pragma once

#include "stdafx.h"
#include "Protocol.h"

class Client
{
public:
	Client();
	~Client();

	void ProcessRecv(int bytes, class OverExpansion* exp);

public:
	int ClientNum;
	SOCKET Socket;

private:
	void Recv();
	void Send(class Packet* packet);

private:
	class OverExpansion* Exp;
	char SendBuffer[CHAT_SIZE];
	int RemainDataLen = 0; // Remaining data length in the send buffer
};

