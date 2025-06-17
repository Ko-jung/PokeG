#pragma once

#include "Protocol.h"

#include <memory>

class PACKET
{
public:
	unsigned short Size;
	char	Type;

	PACKET(char type) : Type(type) { Size = sizeof(PACKET); }
	//virtual void OnExcute(int ClientId) {};
};

struct CS_LOGIN_PACKET : PACKET {
	char	name[NAME_SIZE];

	CS_LOGIN_PACKET() : PACKET(0)
	{ 
		Size = sizeof(CS_LOGIN_PACKET);
		memset(name, 0, NAME_SIZE);
	}
	//virtual void OnExcute(int ClientId);
};

struct CS_MOVS_PACKET : PACKET {
	char	name[NAME_SIZE];

	CS_MOVS_PACKET() : PACKET(0)
	{
		Size = sizeof(CS_MOVS_PACKET);
		memset(name, 0, NAME_SIZE);
	}
};

struct CS_8DIRECT_MOVE_PACKET : PACKET {
	POSITION		Position;
	char			direction;		// See Direction
	unsigned int	move_time;

	CS_8DIRECT_MOVE_PACKET() : PACKET(CS_8DIRECT_MOVE) { Size = sizeof(CS_8DIRECT_MOVE_PACKET); }
};