#include "_level2.h"
#include <cstdio>

_level2::_level2()
{
    //ctor
    myTime->startTime = clock();
    scene = LEVEL2;

}

_level2::~_level2()
{
    //dtor
}

void _level2::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height= GetSystemMetrics(SM_CYSCREEN);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
}

void _level2::initGL()
{
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,0.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers
    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    menuBG.parallaxInit("images/prlx.png");
    cam.camInit();
    bg1.parallaxInit("images/bg_level1.jpg");
    bg2.parallaxInit("images/bg_level2.png");
    bg3.parallaxInit("images/bg_level3.png");
    initGame();
    isInit = true;
}


void _level2::drawText2D(const char* text, int x, int y)
{
    // Save current projection & switch to orthographic 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // 0..width, 0..height in screen pixels

    // Modelview for 2D
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.0f);

    // Note: raster Y is from bottom, so we use y as-is
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.25f, 0.25f, 1.5f);   // increase this number to make text bigger

    for (const char* p = text; *p; ++p)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

    // Restore matrices
    glPopMatrix(); // modelview
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



void _level2::initGame()
{
    currentLevel = 1;
    laneIndex = 1;         // middle lane
    score = 0;
    lives = 3;
    playerX = 0.0f;
    playerY = -1.0f;
    playerZ = -4.0f;
    player.x = playerX;
    player.y = playerY;
    player.z = playerZ;
    player.init("images/horse.md2");
    isJumping = false;
    jumpSpeed = 0.2f;
    gravity   = 0.02f;

    // set up simple obstacles in front of player
    for (int i = 0; i < MAX_OBS; ++i) {
        obstacles[i].active = true;
        obstacles[i].y = -1.0f;              // ground level

        // spread them along Z
        obstacles[i].z = -30.0f - i * 12.0f;

        // random-ish lanes: -2, 0, 2
        int lane = i % 3;                    // 0,1,2
        if (lane == 0) obstacles[i].x = -2.0f;
        if (lane == 1) obstacles[i].x =  0.0f;
        if (lane == 2) obstacles[i].x =  2.0f;
    }
}

void _level2::updateGame()
{
    if (isJumping)
    {
        playerY += jumpSpeed;
        jumpSpeed -= gravity;

        // land on the ground
        if (playerY <= -1.0f)
        {
            playerY   = -1.0f;
            isJumping = false;
            jumpSpeed = 0.2f;   // reset for the next jump
        }
    }


    if      (laneIndex == 0) playerX = -2.0f;
    else if (laneIndex == 1) playerX =  0.0f;
    else                     playerX =  2.0f;

    player.x = playerX;
    player.y = playerY;
    player.z = playerZ;
    player.update(isJumping);


    // -----------------------
    // 3) Level-based speeds
    // -----------------------
    float runSpeed;       // for background scroll
    float obstacleSpeed;  // for cubes

    if (currentLevel == 1) {
        runSpeed      = 0.25f;
        obstacleSpeed = 0.10f;
    }
    else if (currentLevel == 2) {
        runSpeed      = 0.30f;
        obstacleSpeed = 0.14f;
    }
    else { // level 3+
        runSpeed      = 0.35f;
        obstacleSpeed = 0.18f;
    }

    float texBase = runSpeed * 0.008f;

    bg1.prlxScrollAuto("left", texBase * 0.5f);   // far
    bg2.prlxScrollAuto("left", texBase * 0.75f);  // mid
    bg3.prlxScrollAuto("left", texBase * 1.0f);   // ground


    for (int i = 0; i < MAX_OBS; ++i)
    {
        if (!obstacles[i].active) continue;

        // move toward player
        obstacles[i].z += obstacleSpeed;

        // A) if obstacle passes the player without collision → score++
        if (obstacles[i].z > playerZ + 5.0f)
        {
            // give points for dodging
            score++;

            // level up thresholds
            if (score == 10) currentLevel = 2;
            else if (score == 20) currentLevel = 3;

            // recycle far ahead
            obstacles[i].z = -50.0f;

            int lane = rand() % 3;
            if (lane == 0) obstacles[i].x = -2.0f;
            if (lane == 1) obstacles[i].x =  0.0f;
            if (lane == 2) obstacles[i].x =  2.0f;
        }

        // B) collision check
        float dx = obstacles[i].x - playerX;
        if (dx < 0) dx = -dx;

        float dz = obstacles[i].z - playerZ;
        if (dz < 0) dz = -dz;

        // tune these based on horse size + cube size
        const float hitX = 0.8f;
        const float hitZ = 1.5f;

        if (dx < hitX && dz < hitZ)
        {
            // collision!
            lives--;

            // simple reaction: reset obstacle and reposition player in middle lane
            laneIndex = 1;
            playerX = 0.0f;

            // push this obstacle far away again
            obstacles[i].z = -50.0f;
            int lane = rand() % 3;
            if (lane == 0) obstacles[i].x = -2.0f;
            if (lane == 1) obstacles[i].x =  0.0f;
            if (lane == 2) obstacles[i].x =  2.0f;

            // if no lives left → back to menu (you can also do GAME OVER state)
            if (lives <= 0)
            {
                gameState = STATE_MENU;
                 initGame();
            }
        }
    }

}


void _level2::drawGame()
{
    glLoadIdentity();


    // ---- background (level 1) ----
    glPushMatrix();
    glScalef(4.33f, 4.33f, 1.0f);
    if (currentLevel == 1)
        bg1.drawParallax(width, height);
    else if (currentLevel == 2)
        bg2.drawParallax(width, height);
    else
        bg3.drawParallax(width, height);
    glPopMatrix();

    // ---- player
    player.x = playerX;
    player.y = playerY;
    player.z = playerZ;

    player.draw();

    // ---- obstacles ----
    glColor3f(1.0f, 0.3f, 0.3f);    // red cubes
    for (int i = 0; i < MAX_OBS; ++i) {
        if (!obstacles[i].active) continue;
        glPushMatrix();
        glTranslatef(obstacles[i].x, obstacles[i].y, obstacles[i].z);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    // Hud to display live and score
    char hud[64];
    sprintf(hud, "LEVEL: %d   SCORE: %d   LIVES: %d", currentLevel, score, lives);
    drawText2D(hud, 40, height - 60);   // top-left-ish
}

void _level2::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
glTranslatef(0, 0, -10);
        updateGame();
        drawGame();

}



void _level2::mouseMapping(int x, int y)
{
    GLint viewPort[4];
    GLdouble ModelViewM[16];
    GLdouble projectionM[16];
    GLfloat winX,winY,winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT,viewPort);

    winX =(GLfloat)x;
    winY = (GLfloat)y;

    glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject(winX,winY,winZ,ModelViewM,projectionM,viewPort,&msX,&msY,&msZ);
}




int _level2::winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:
{
    input.wParam = wParam;

    if (wParam == VK_LEFT) {
        // move left lane (0,1,2)
        if (laneIndex > 0) laneIndex--;
    }
    else if (wParam == VK_RIGHT) {
        // move right lane (0,1,2)
        if (laneIndex < 2) laneIndex++;
    }
    else if (wParam == VK_UP || wParam == VK_SPACE) {
        // jump
        if (!isJumping) {
            isJumping = true;
            // jumpSpeed already set in initGame
        }
    }


    return 0;
    break;
}


        case WM_KEYUP:

        break;

        case WM_LBUTTONDOWN:


             mouseMapping(LOWORD(lParam), HIWORD(lParam));
             if (gameState == STATE_LANDING) {
                        gameState = STATE_MENU;
                    }
                    else if (gameState == STATE_MENU) {

                        float x = (float)msX;
                        float y = (float)msY;

                        // These ranges correspond to the Y positions we used in drawMenu()
                        if (y > 0.25f && y < 0.35f) {          // near NEW GAME
                            gameState = STATE_PLAYING;
                        }
                        else if (y > -0.05f && y < 0.05f) {    // near HELP
                            gameState = STATE_HELP;
                        }
                        else if (y > -0.35f && y < -0.25f) {   // near EXIT
                            PostQuitMessage(0);
                        }
                    }
            break;

        case WM_RBUTTONDOWN:

            break;

         case WM_MBUTTONDOWN:


            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:

            break;

        case WM_MOUSEMOVE:

            break;
        case WM_MOUSEWHEEL:

            break;

        default:
            break;

    }
    return 0;
}
