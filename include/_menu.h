#ifndef _MENU_H
#define _MENU_H

#include "_scene.h"
// #include <_scenemanager.h>
#include <string>
#include <Windows.h>

class _scenemanager;
extern _scenemanager sceneManager;


class _Menu : public _Scene {
public:

    _Menu();
    ~_Menu();

    void initGL() override;
    void drawScene() override;
    int winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void reSizeScene(int w, int h) override;

private:
    void drawText(float x, float y, const std::string& text);
    void handleMouse(int x, int y, bool click);

    int width, height;
};

#endif // _MENU_H
