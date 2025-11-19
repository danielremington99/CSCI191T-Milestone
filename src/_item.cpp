#include "_item.h"

_item::_item()
{
    stats["Health"]         = 0.0f;
    stats["MovementSpeed"]  = 0.0f;
    stats["Damage"]         = 0.0f;
    stats["AttackSpeed"]    = 0.0f;
    stats["CriticalChance"] = 0.0f;
    stats["Armor"]          = 0.0f;
    stats["ArmorPiercing"]   = 0.0f;
}

_item::~_item()
{
    //dtor
}
