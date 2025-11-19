#ifndef _LEVEL_1_H
#define _LEVEL_1_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include<_player.h>
#include<_inputs.h>
#include<_textureLoader.h>
#include<_parallax.h>
#include<_timer.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_3DModelLoader.h>
#include<_camera.h>
#include<_bullets.h>
#include<_collisionCheck.h>
#include<_sounds.h>
#include<_scene.h>

class _level1 : public _Scene
{
    public:
        _level1();           //Constructor
        virtual ~_level1();  //Destructor

        _light *myLight = new _light();           //light settings
        _model *myModel = new _model();           //create a model
        _inputs *myInput = new _inputs();         //input handler
        _textureLoader *myTexture = new _textureLoader(); //for loading images
        _parallax *myPrlx = new _parallax();
        _parallax *myPrlx2 = new _parallax();    //optional second parallax layer
        _skyBox *mySkyBox = new _skyBox();
        _sprite *mySprite = new _sprite();
        _timer *myTimer = new _timer();
        _camera *myCam = new _camera();
        _collisionCheck *myCol = new _collisionCheck();

        _player *mdl3D = new _player();          //level-specific player model
        _3DModelLoader *mdl3DW = new _3DModelLoader();

        _sounds *snds = new _sounds();           //level-specific sounds

        _bullets b[10];                           //array of bullets
        _model fallingTeapots[10];                //optional, to match _Scene
        int clickCnt = 0;
        int teapotFalling = 0;
        bool escapeUsed = true;

    void reSizeScene(int width, int height) override;
    void initGL() override;
    void drawScene()override;
    int winMSG(HWND, UINT, WPARAM, LPARAM);

        void mouseMapping(int, int);
        double msX, msY, msZ;

        int width, height; //keep record of the screen size

    protected:

    private:

};

#endif // _LEVEL_1_H

