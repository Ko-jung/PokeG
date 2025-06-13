#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"

class Monster
{
public:
    Monster();
    ~Monster();

    virtual void ParseData(const char* data, int size);
    virtual void SpawnMonster() {};

public:
    int     Hp;
    int     Attack;
    int     Defense;
    int     Speed;
    float   Size;
};

Monster::Monster()
{
    Hp = 0;
    Attack = 0;
    Defense = 0;
    Speed = 0;
    Size = 0.0f;
}

Monster::~Monster()
{
}

void Monster::ParseData(const char* data, int size)
{

}

int main()
{
    std::ifstream file("Data.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << std::endl;
        return 1;
    }

    // Data.json의 모든 내용을 불러오기
    std::stringstream buffer;
    buffer << file.rdbuf();

	// JSON 문자열을 rapidjson::Document로 파싱하기
    std::string jsonStr = buffer.str();
    rapidjson::Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError()) {
        std::cerr << "JSON parse error!" << std::endl;
        return false;
    }

    if (!doc.HasMember("Monsters") || !doc["Monsters"].IsObject()) {
        std::cerr << "'Monsters' field missing or not an Object!" << std::endl;
        return false;
    }

    
    const auto& monsters = doc["Monsters"];
    for (auto it = monsters.MemberBegin(); it != monsters.MemberEnd(); ++it) {
        std::string name = it->name.GetString();
        const auto& m = it->value;

        Monster monster;
        monster.Hp = m["Hp"].GetInt();
        monster.Speed = m["Speed"].GetInt();
    }
}