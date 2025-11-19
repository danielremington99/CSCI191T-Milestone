#ifndef _PLAYER2_H
#define _PLAYER2_H

#include <_common.h>
#include <_3dmodelloader.h>

class _player2
{
    public:
        _player2();
         ~_player2();

    _3DModelLoader model;   // MD2 loader

    float x, y, z;          // world position
    float scale;            // size of the horse

    // load MD2 model from file
    void init(const char* md2File);

    // update animation + sync position
    void update(bool jumping);

    // draw the player
    void draw();

    protected:

    private:
};

#endif // _PLAYER2_H
