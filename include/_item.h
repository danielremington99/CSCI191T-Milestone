#ifndef _ITEM_H
#define _ITEM_H

#include <_common.h>
#include <_sprite.h>


class _item : public _sprite
{
    public:
        _item();
        virtual ~_item();

        std::unordered_map<std::string,float> getStats() {return stats;}
        float getStat(std::string statName){return stats[statName];}

        void pickupItem();

        Type type;
        int amount, id;
        std::unordered_map<std::string,float> stats;
    protected:

    private:
};

#endif // _ITEM_H
