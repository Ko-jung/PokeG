#pragma once

#include "stdafx.h"

class Monster
{
public:
    Monster();
    ~Monster();

    virtual void ParseData(const char* data, int size);
    virtual void SpawnMonster() = 0;

protected:
	std::string Name;
    int         Hp;
    int         Attack;
    int         Defense;
    int         Speed;
    float       Size;
};

