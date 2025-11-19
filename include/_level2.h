#ifndef _LEVEL_2_H
#define _LEVEL_2_H

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
#include <_scene.h>
#include<_player2.h>


class _level2 : public _Scene
{
        public:
        _level2();           //constructor
        virtual ~_level2();  //Destructor

        int clickCnt =0;

        void reSizeScene(int width, int height);  // resize window
        void initGL();                            // initialize GL graphics
        void drawScene();                         // render scene
        int winMSG(HWND,UINT,WPARAM,LPARAM);      // to get keyboard interrupts and pass it to inputs
        void mouseMapping(int,int);
        double msX,msY,msZ;
        _inputs input;

        float yNew  = 0.0f;
        float yHelp = 0.0f;
        float yExit = 0.0f;
        // camera
        _camera cam;

        // backgrounds for 3 levels
        _parallax bg1;
        _parallax bg2;
        _parallax bg3;
        enum GameState {
        STATE_LANDING,
        STATE_MENU,
        STATE_HELP,
        STATE_PLAYING,
        STATE_PAUSE
        };
        enum MenuItem {
            MENU_NEW = 0,
            MENU_HELP = 1,
            MENU_EXIT = 2
        };
        _sprite landingSprite;      // separate image for landing page
        _parallax menuBG;           // background behind the menu

        int menuSelected = MENU_NEW;

        GameState gameState = STATE_LANDING;

        void drawText2D(const char* text, int x, int y);
        void drawStrokeTextOutlined(const char* text, float x, float y, float scale);
        void drawStrokeText(const char* text, float x, float y, float scale);

        void drawLanding();
        void drawMenu();
        void drawHelp();
                // ---- GAMEPLAY STATE ----
        int currentLevel = 1;

        // player position
        float playerX = 0.0f;     // left/right lane
        float playerY = 0.0f;     // jump height
        float playerZ = -8.0f;    // fixed distance from camera

        // jump physics
        bool  isJumping   = false;
        float jumpSpeed   = 0.35f;
        float gravity     = 0.02f;

        // lanes: -2, 0, +2 for left, middle, right
        int laneIndex = 1;        // 0 = left, 1 = mid, 2 = right

        // obstacles (simple cubes for now)
        struct Obstacle {
            float x, y, z;
            bool active;
        };
        static const int MAX_OBS = 6;
        Obstacle obstacles[MAX_OBS];
        void initGame();
        void updateGame();
        void drawGame();
        void drawPause();
        _player2 player;
        int width, height;  // keep record of the screen size

        int   score;
        int   lives;
    protected:

    private:
};

#endif // _LEVEL_2_H
