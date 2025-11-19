#include "_scenemanager.h"
#include <iostream>

_scenemanager::_scenemanager() {

    // Create scenes
    menu   = new _Menu();
    level1 = new _level1();
    level2 = new _level2();
    level3 = new _level3();

    // Start in main menu
    currentSceneEnum = MAIN;
    currentScene = menu;

    keyMPressed = false;
}

_scenemanager::~_scenemanager() {
    delete menu;
    delete level1;
    delete level2;
    delete level3;
}

void _scenemanager::switchScene() {

    previousScene = currentScene;

    switch (currentSceneEnum) {
        case MAIN:   currentScene = menu;   break;
        case LEVEL1: currentScene = level1; break;
        case LEVEL2: currentScene = level2; break;
        case LEVEL3: currentScene = level3; break;
        default:     currentScene = menu;   break;
    }

    // If scene changed, force reinitialization
    if (currentScene != previousScene) {
        currentScene->isInit = false;
        std::cout << "Switching scene to enum value: "
                  << currentSceneEnum << "\n";
    }
}

void _scenemanager::cycleSceneWithMKey() {

    // Cycle through all scenes when M is pressed
    switch (currentSceneEnum) {
        case MAIN:   currentSceneEnum = LEVEL1; break;
        case LEVEL1: currentSceneEnum = LEVEL2; break;
        case LEVEL2: currentSceneEnum = LEVEL3; break;
        case LEVEL3: currentSceneEnum = MAIN;   break;
    }

    switchScene();
}

void _scenemanager::initScene() {
    if (!currentScene) return;
    currentScene->initGL();
}

void _scenemanager::drawScene() {

    if (!currentScene) return;

    // Detect if the scene class changed its scene enum
    if (currentSceneEnum != currentScene->getScene()) {
        currentSceneEnum = currentScene->getScene();
        switchScene();
    }

    // Initialize if not initialized
    if (!currentScene->isInit)
        initScene();

    // Draw
    currentScene->drawScene();
}

void _scenemanager::reSizeScene(int w, int h) {
    if (currentScene)
        currentScene->reSizeScene(w, h);
}

void _scenemanager::winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    // ----- HANDLE "M" KEY SWITCHING -----
    if (uMsg == WM_KEYDOWN && wParam == 'M') {

        if (!keyMPressed) {            // only fire once
            keyMPressed = true;
            cycleSceneWithMKey();
        }
        return;
    }

    if (uMsg == WM_KEYUP && wParam == 'M') {
        keyMPressed = false;
        return;
    }

    // Pass to current scene
    if (currentScene)
        currentScene->winMSG(hWnd, uMsg, wParam, lParam);
}
