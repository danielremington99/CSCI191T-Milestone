#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <_scene.h>
#include <_level1.h>
#include <_level2.h>
#include <_level3.h>
#include <_menu.h>


class _scenemanager
{
    public:
        _scenemanager();
        virtual ~_scenemanager();
        void switchScene();
        _Scene* previousScene = nullptr;
        void reSizeScene(int width, int height);
        void initScene();
        void drawScene();
        void winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool keyMPressed;
        void cycleSceneWithMKey();

        Scene currentSceneEnum;
    protected:

    private:
    _level1* level1;
    _level2* level2;
    _level3* level3;
    _Menu*   menu;
    _Scene* currentScene = nullptr;
};

#endif // _SCENEMANAGER_H
