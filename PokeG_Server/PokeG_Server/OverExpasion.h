#pragma once
#include "stdafx.h"
#include "Protocol.h"

class OverExpansion
{
public:
	WSAOVERLAPPED _over;
	WSABUF _wsabuf;
	char _send_buf[CHAT_SIZE];
	COMP_TYPE _comp_type;
	int _ai_target_obj;
};