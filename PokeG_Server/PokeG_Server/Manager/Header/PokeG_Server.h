#pragma once

class PokeG_Server
{
	std::shared_ptr<class SOCKET> ListenSocket;
public:
	PokeG_Server();
	~PokeG_Server();

	bool Init();
	void Start();

private:
	int WorkerNum;
};

