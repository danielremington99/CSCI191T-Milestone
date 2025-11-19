#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_timer.h>
#include<_3dmodelloader.h>
#include<_camera.h>
#include<_bullets.h>
#include<_collisioncheck.h>
#include<_sounds.h>


class _Scene
{
    public:
        _Scene(); //Constructor
        virtual ~_Scene(); //Destructor

        _light *myLight = new _light(); //light settings
        _model *myModel = new _model(); //create a model
        _inputs *myInput = new _inputs(); //input handler
        _textureLoader *myTexture = new _textureLoader(); //for loading images
        _parallax *myPrlx = new _parallax();
        _parallax *myPrlx2 = new _parallax();
        _skyBox *mySkyBox = new _skyBox();
        _sprite *mySprite = new _sprite();
        _timer *myTime = new _timer();
        _camera *myCam = new _camera();
        _collisionCheck *myCol = new _collisionCheck();
        _3DModelLoader *mdl3D = new _3DModelLoader();
        _3DModelLoader *mdl3DW = new _3DModelLoader();
        _sounds *snds = new _sounds();
        _bullets b[10];
        int clickCnt = 0;


        virtual void reSizeScene(int width, int height) = 0;  // resize window
        virtual void initGL() = 0;                            // initialize GL graphics
        virtual void drawScene() = 0;
        virtual Scene getScene(){return scene;}
                                 // render scene
        virtual int winMSG(HWND,UINT,WPARAM,LPARAM) = 0;      // to get keyboard interrupts and pass it to inputs
        bool active;
        bool isInit = false;

                void mouseMapping(int, int);
        double msX, msY, msZ;

        int width, height; //keep record of the screen size

        Scene scene;
    protected:

    private:
};

#endif // _SCENE_H
