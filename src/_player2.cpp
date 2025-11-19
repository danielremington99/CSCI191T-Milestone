#include "_player2.h"

_player2::_player2()
{
    //
        x = 0.0f;
    y = -2.0f;
    z = -8.0f;
    scale = 0.007f;
}

_player2::~_player2()
{
    //dtor
}
void _player2::init(const char* md2File)
{
    // load MD2 model
    model.initModel(md2File);
}

void _player2::update(bool jumping)
{
        // For now ignore RUN/JUMP ranges; just animate across all frames.
    // Only keep position in sync.
    model.pos.x = x;
    model.pos.y = y;
    model.pos.z = z;
}

void _player2::draw()
{
        glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);
     glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    model.Draw();   // MD2 animation over full range

    glPopMatrix();
}


