#ifndef _ENEMY_H
#define _ENEMY_H

#include <_common.h>
#include <_3dmodelloader.h>


class _enemy : public _3DModelLoader
{
    public:
        enum EnemyAction {
            STAND,
            WALK,
            ATTACK
        };
        _enemy();
        virtual ~_enemy();
        void init(const char* filename);
        void draw();

        void spawn(vec3 center);

        void moveTowardPoint(vec3 point);

        bool isAlive, isSpawned;
        float angle;

        //Animation
        EnemyAction action = STAND;
        float pframe = 0.0f;

        int damage, health;
        float speed;

    protected:

    private:
};

#endif // _ENEMY_H
