#pragma once
#include "../stdafx.h"

class JsonParser
{
public:
	JsonParser();
	~JsonParser();

	virtual void ParseData(std::string DataName) = 0;

private:
	class Document* document;
};

