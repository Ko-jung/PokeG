#pragma once

#include "../../stdafx.h"

class PokeG_Server
{
	SOCKET ListenSocket;
public:
	PokeG_Server();
	~PokeG_Server();

	bool Init();
	void Start();
};

