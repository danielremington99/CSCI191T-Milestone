#include "_menu.h"
#include <GL/gl.h>
#include <iostream>

_Menu::_Menu() : width(800), height(600) {
    scene = MAIN;
    active = true;
}

_Menu::~_Menu() {}

void _Menu::initGL() {
    std::cout << "Initializing Menu\n";

    isInit = true;

    HDC hDC = wglGetCurrentDC();
    HFONT font = CreateFontA(
        -24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
        "Arial"
    );

    SelectObject(hDC, font);
    wglUseFontBitmapsA(hDC, 0, 256, 1000);
}

void _Menu::drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    glListBase(1000);
    glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());
}

void _Menu::drawScene() {
    glViewport(0, 0, width, height);

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Background
    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.3f);
        glVertex2f(-1, -1);
        glVertex2f( 1, -1);
        glVertex2f( 1,  1);
        glVertex2f(-1,  1);
    glEnd();

    glColor3f(1,1,1);
    drawText(-0.2f, 0.4f, "NEW GAME");
    drawText(-0.2f, 0.0f, "HELP");
    drawText(-0.2f, -0.4f, "QUIT");

    // restore
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void _Menu::handleMouse(int x, int y, bool click) {
    if (!active || !click) return;

    float ndcY = 1.0f - (2.0f * y / (float)height);
    float h = 0.1f;

    if (ndcY > 0.4f - h && ndcY < 0.4f + h) {
        scene = LEVEL1;
        active = false;
        std::cout << "Start Game\n";
    }
    else if (ndcY > -h && ndcY < h) {
        scene = CREDITS;
        active = false;
        std::cout << "Show Help\n";
    }
    else if (ndcY > -0.4f - h && ndcY < -0.4f + h) {
        scene = QUIT;
        std::cout << "Quit Game\n";
        PostQuitMessage(0);
    }
}

int _Menu::winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            handleMouse(x, y, true);
        } break;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                scene = MAIN;
            }
            break;
    }
    return 0;
}

void _Menu::reSizeScene(int w, int h) {
    width = w;
    height = h;
}
