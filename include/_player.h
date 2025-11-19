#ifndef _PLAYER_H
#define _PLAYER_H

#include <_3dmodelloader.h>
#include <_common.h>


class _player : public _3DModelLoader
{
    public:
        _player();
        virtual ~_player();
        void draw();
        void init(const char* model);

        std::unordered_map<std::string,float> stats;

    protected:

    private:
};

#endif // _PLAYER_H
