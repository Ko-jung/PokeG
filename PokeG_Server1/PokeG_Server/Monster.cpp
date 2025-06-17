#include "Monster.h"

#include "Util/JsonParser.h"

Monster::Monster()
	: Name("Unknown"), Hp(0), Attack(0), Defense(0), Speed(0), Size(0.0f)
{
}

Monster::~Monster()
{
}

void Monster::ParseData(const char* data, int size)
{

}
