#include "JsonParser.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"

#include "UtilLog.h"

JsonParser::JsonParser()
{
    // read json file
    std::fstream JsonFile("Data.json", std::ios::in);
    if (!JsonFile.is_open()) {
		UtilLog::Log("Data.json file not exist");
        return;
    }


}

JsonParser::~JsonParser()
{
}
