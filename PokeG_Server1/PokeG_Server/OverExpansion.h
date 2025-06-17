#pragma once

#include "stdafx.h"
#include "Protocol.h"
#include "Enum.h"

class OverExpansion
{
public:
	WSAOVERLAPPED Over;
	WSABUF Wsabuf;
	char SendBuf[CHAT_SIZE];
	COMP_TYPE CompType;

	OverExpansion()
	{
		Wsabuf.len = CHAT_SIZE;
		Wsabuf.buf = SendBuf;
		CompType = COMP_TYPE::OP_RECV;
		ZeroMemory(&Over, sizeof(Over));
	}
	OverExpansion(char* packet)
	{
		Wsabuf.len = *(unsigned short*)packet;
		Wsabuf.buf = SendBuf;
		ZeroMemory(&Over, sizeof(Over));
		CompType = COMP_TYPE::OP_SEND;
		memcpy(SendBuf, packet, Wsabuf.len);
	}
};