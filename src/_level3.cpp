#include "_level3.h"

_level3::_level3()
{
    //ctor
    myTime->startTime = clock();
    scene = LEVEL1;
    isInit = false;
    isMovingLeft = isMovingRight = isMovingForward = isMovingBack = false;
    isMoving = false;
    enemies.resize(100);
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i] = new _enemy();
    }
}

_level3::~_level3()
{
    //dtor
}

void _level3::reSizeScene(int width, int height)
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

void _level3::initGL()
{
    ShowCursor(FALSE);
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,1.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers

    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width / (float)height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    myTexture->loadTexture("images/tex.jpg");
    // myFloor -> loadTexture("images/");
    myPrlx->parallaxInit("images/prlx.jpg");

    mySkyBox->skyBoxInit();
    mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/front.png");
    mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/back.png");
    mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/top.png");
    mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/bottom.png");
    mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/right.png");
    mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/left.png");
    mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.png");

    mySprite->spriteInit("images/eg.png",6,4);
    mdl3D->initModel("../AssetsDownload/badboyblake/tris.md2");
    mdl3DW->initModel("models/Tekk/weapon.md2");
    for(int i = 0; i < enemies.size(); i++){
        enemies.at(i)->init("models/Tekk/tris.md2");
    }


    myCam->camInit();

    snds->initSounds();
    snds->playMusic("sounds/HighNoon.mp3");
    isInit = true;
}

void _level3::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    // Player and Camera angles Calculations
    // TODO: move into Player and Camera
    float speed = 0.05f;
    vec3 lookDir;
    lookDir.x = myCam->des.x - myCam->eye.x;
    lookDir.y = 0;
    lookDir.z = myCam->des.z - myCam->eye.z;

    float len = sqrtf(lookDir.x * lookDir.x + lookDir.z * lookDir.z);
    if(len > 0.001f) {
        lookDir.x /= len;
        lookDir.z /= len;
    }

    vec3 moveVec = {0, 0, 0};
    if(isMovingForward)  { moveVec.x += lookDir.x; moveVec.z += lookDir.z; }
    if(isMovingBack)     { moveVec.x -= lookDir.x; moveVec.z -= lookDir.z; }
    if(isMovingLeft)     { moveVec.x += lookDir.z; moveVec.z -= lookDir.x; }
    if(isMovingRight)    { moveVec.x -= lookDir.z; moveVec.z += lookDir.x; }

    float moveLen = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    if(moveLen > 0.001f) {
        moveVec.x /= moveLen;
        moveVec.z /= moveLen;

        mdl3D->pos.x += moveVec.x * speed;
        mdl3D->pos.z += moveVec.z * speed;
        isMoving = true;
    } else {
        isMoving = false;
    }

    float targetAngle = currentPlayerAngle;;
    if(isMoving) {
        targetAngle = -atan2f(moveVec.x, -moveVec.z) * 180.0f / PI;
    }
    // Smooth the turns out
    float rotationSpeed = 1.0f;
    float angleDiff = targetAngle - currentPlayerAngle;

    while(angleDiff > 180.0f) angleDiff -= 360.0f;
    while(angleDiff < -180.0f) angleDiff += 360.0f;

    if(fabs(angleDiff) < rotationSpeed)
        currentPlayerAngle = targetAngle;
    else
        currentPlayerAngle += (angleDiff > 0 ? rotationSpeed : -rotationSpeed);

    if(fabs(angleDiff) < rotationSpeed)
        currentPlayerAngle = targetAngle;
    else
        currentPlayerAngle += (angleDiff > 0 ? rotationSpeed : -rotationSpeed);
    myCam->des.x = mdl3D->pos.x;
    myCam->des.y = mdl3D->pos.y + 3.0f;
    myCam->des.z = mdl3D->pos.z;

    myCam->rotateXY();
    myCam->eye.y += 5.0f;
    myCam->setUpCamera();

    // Set all sprites to look at player
    // TODO: move into sprite
    vec3 toPlayer;
    toPlayer.x = mdl3D->pos.x - mySprite->pos.x;
    toPlayer.y = 0; // ignore vertical rotation for 2D sprites
    toPlayer.z = mdl3D->pos.z - mySprite->pos.z;
    float angle = atan2f(toPlayer.x, toPlayer.z) * 180.0f / PI;

    // Collision Check to Sprites, to pick up items
    std::cout << myCol->isSphereCol(mdl3D->pos,mySprite->pos,1.0f,1.0f,0.1f) << std::endl;

    //Set up enemy waves
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    enemiesPerWave = rand()%15;
    if(currentTime - lastWaveTime >= waveInterval){
        int spawned = 0;
        for(auto& e : enemies) {
            if(!e->isSpawned && spawned < enemiesPerWave){
                e->spawn(mdl3D->pos);
                spawned++;
            }
        }
        lastWaveTime = currentTime;
    }
    for(auto& e : enemies) {
        if(e->isSpawned && e->isAlive){
            e->moveTowardPoint(mdl3D->pos);
        }
    }
    // Auto attack handler
    if(currentTime - lastAttackTime >= mdl3D->stats["AttackSpeed"]){

    }

    //Begin Drawing

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glPushMatrix();
        myTexture->bindTexture();
       // myModel->drawModel();
    glPopMatrix();

    // Background Rendering
    glPushMatrix();
        glScalef(4.33,4.33,1);
        myPrlx->drawParallax(width,height);
        myPrlx->prlxScrollAuto("left", 0.0005);
        glDepthMask(GL_FALSE);
        mySkyBox->drawSkyBox();
        glDepthMask(GL_TRUE);
    glPopMatrix();

    glPushMatrix();
        //glTranslatef(mySprite->pos.x, mySprite->pos.y, mySprite->pos.z);
        glRotatef(angle, 0, 1, 0);
        glScalef(1.0f,1.0f,1.0f);
        mySprite->drawSprite();
      // mySprite->actionTrigger = mySprite->WALKRIGHT;

        if(myTime->getTicks()>70)
        {
            mySprite->spriteActions();
            myTime->reset();
        }
    glPopMatrix();
    glPushMatrix();
        for(auto& e : enemies) if(e->isAlive)e->draw();
    glPopMatrix();

    glPushMatrix();
        // Player movement
        // Move player
        glTranslatef(mdl3D->pos.x,mdl3D->pos.y,mdl3D->pos.z);
        // Rotate player
        glRotatef(currentPlayerAngle-90, 0, 1, 0);
        // Rotate model to face movement direction

        // Model rotation fixes
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        if(isMoving){
            if(mdl3D->actionTrigger != mdl3D->RUN) {
                mdl3D->actionTrigger = mdl3D->RUN;
                mdl3D->pframe = 0;
            }
        } else {
            if(mdl3D->actionTrigger != mdl3D->STAND){
                mdl3D->actionTrigger = mdl3D->STAND;
                mdl3D->pframe = 0;
            }
        }

        glScalef(0.1,0.1,0.1);
        mdl3D->Actions();
        mdl3DW->Actions();
        mdl3DW->Draw();
        mdl3D->Draw();
    glPopMatrix();

    glPushMatrix();

       for(int i =0; i<10;i++)
       {
           if(b[i].live)
           {
               b[i].drawBullet();
               b[i].bulletActions();
           }
       }
    glPopMatrix();
}


void _level3::mouseMapping(int x, int y)
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

int _level3::winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            switch(wParam) {
                case 'W':
                case 'w':
                    isMovingForward = true;
                    break;
                case 'A':
                case 'a':
                    isMovingLeft = true;
                    break;
                case 'D':
                case 'd':
                    isMovingRight = true;
                    break;
                case 'S':
                case 's':
                    isMovingBack = true;
                    break;
                case VK_SPACE:
                    isJumping = true;
                    break;
            }
            // Player movement

            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            // myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            // myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            switch(wParam) {
                case 'W':
                case 'w':
                    isMovingForward = false;
                    break;
                case 'A':
                case 'a':
                    isMovingLeft = false;
                    break;
                case 'D':
                case 'd':
                    isMovingRight = false;
                    break;
                case 'S':
                case 's':
                    isMovingBack = false;
                    break;
                case VK_SPACE:
                    isJumping = false;
            }
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            // mdl3D->actionTrigger=mdl3D->STAND;
            // mdl3DW->actionTrigger=mdl3DW->STAND;
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));

             mouseMapping(LOWORD(lParam), HIWORD(lParam));
             clickCnt =clickCnt%10;

                 b[clickCnt].src.x = mdl3D->pos.x;
                 b[clickCnt].src.y = mdl3D->pos.y;
                 b[clickCnt].src.z = mdl3D->pos.z;

                 b[clickCnt].des.x = msX;
                 b[clickCnt].des.y = -msY;
                 b[clickCnt].des.z = msZ;

                 b[clickCnt].t =0;
                 b[clickCnt].actionTrigger = b[clickCnt].SHOOT;
                 b[clickCnt].live = true;
                   clickCnt++;
        break;

        case WM_RBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

         case WM_MBUTTONDOWN:
             myInput->wParam = wParam;
             myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));

            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            myInput->wParam = wParam;
            myInput->mouseEventUp();
            break;

        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            // Calculate movement delta relative to the screen center
            int centerX = width / 2;
            int centerY = height / 2;

            int dx = x - centerX;
            int dy = y - centerY;

            myCam->rotAngle.x += dx * 0.2f;
            myCam->rotAngle.y -= dy * 0.2f;

            if(myCam->rotAngle.y > 85) myCam->rotAngle.y = 85;
            if(myCam->rotAngle.y < 0 ) myCam->rotAngle.y = 0;

            // Reset the mouse to the center of the window
            POINT centerPoint = { centerX, centerY };
            ClientToScreen(hWnd, &centerPoint);
            SetCursorPos(centerPoint.x, centerPoint.y);

            lastMouseX = centerX;
            lastMouseY = centerY;

            myInput->wParam = wParam;
            myInput->mouseMove(myModel, x, y);

            // Lock cursor to window client area
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            POINT ul = { rcClient.left, rcClient.top };
            POINT lr = { rcClient.right, rcClient.bottom };
            ClientToScreen(hWnd, &ul);
            ClientToScreen(hWnd, &lr);
            RECT clipRect = { ul.x, ul.y, lr.x, lr.y };
            ClipCursor(&clipRect);
            ShowCursor(FALSE);
        }
            break;
        case WM_MOUSEWHEEL:
              myInput->wParam = wParam;
              myInput->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;

        default:
            break;

    }
    return 0;
}
