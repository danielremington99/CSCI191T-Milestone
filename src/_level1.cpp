#include "_level1.h"

_level1::_level1()
{
    //ctor
    myTime->startTime = clock();
    scene = LEVEL1;
    isInit = false;
                    for (int i = 0; i < 10; i++)
            {
            fallingTeapots[i].posX = (rand() % 20 - 10) * 0.2f;  // random X spread
            fallingTeapots[i].posY = 5.0f + (rand() % 100) * 0.05f; // high in sky
            fallingTeapots[i].posZ = -15.0f; // in front of camera
            }
}

_level1::~_level1()
{
    //dtor
}

void _level1::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    //this->width = GetSystemMetrics(SM_CXSCREEN);
    //this->height= GetSystemMetrics(SM_CYSCREEN);

    this->width = width;
    this->height = height;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
}

void _level1::initGL()
{
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
mySkyBox->skyBoxInit();
            mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/nx.jpg");
            mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/px.jpg");
            mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/nz.jpg");
            mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/pz.jpg");
            mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/ny.jpg");
            mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/py.jpg");
            mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.jpg");

            mySprite->spriteInit("images/eg.png",6,4);
            mdl3D->initModel("models/Tekk/weapon4.md2");
            //mdl3DW->initModel("models/Tekk/weapon.md2");
            myCam->camInit();
            myCam->eye.x = mdl3D->pos.x;
            myCam->eye.y= mdl3D->pos.y + 1.0f;
            myCam->eye.z= mdl3D->pos.z;
            myCam->des.x = mdl3D->pos.x;
            myCam->des.y= mdl3D->pos.y + 1.0f;
            myCam->des.z = mdl3D->pos.z-1.0f; // always forward
            myCam->setUpCamera();
snds->initSounds();



    isInit = true;
}

void _level1::drawScene()
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
    glLoadIdentity();
glPushMatrix();
            //myCam->setUpCamera();
            mySkyBox->drawSkyBox();
            glTranslatef(-.5f, -1.5f, -2.5f);  // X=right, Y=down, Z=back
            glScalef(0.1f, 0.1f, 0.1f);
            //mdl3D->Actions();
            //mdl3DW->Draw();
            mdl3D->Draw();
            glTranslatef(-.5f, -1.5f, -2.5f);
            //mdl3DW->Actions();
            glPopMatrix();

            glPushMatrix();



            fallingTeapots[teapotFalling].posY -= 0.004f;

            if (fallingTeapots[teapotFalling].posY < -3.0f)
            {

                teapotFalling = (teapotFalling + 1) % 10;
                fallingTeapots[teapotFalling].posY = 10.0f + (rand() % 20) * 0.05f;
                fallingTeapots[teapotFalling].posX = (rand() % 20) * 0.3f;
                fallingTeapots[teapotFalling].posZ = -25.0f;
            }

            bool collisionHit;
            for(int i =0; i<10; i++)
            {
                if(b[i].live)
                {
                    b[i].drawBullet();
                    b[i].bulletActions();
                    //do collision check between bullet and model

                    collisionHit = myCol ->isSphereCol(fallingTeapots[i].p,b[i].pos, 1.5f, 0.1f, 0.0f); //NOT GOING TO WORK SINCE WE NEED TO HAVE VEC3 IN MODEL
                    if(collisionHit){
                        fallingTeapots[i].posZ = -500.0f;
                        b[i].live = false;
                    }

                }
            }

            if(collisionHit == false){
                fallingTeapots[teapotFalling].drawModel();
            }
            glPopMatrix();

}

void _level1::mouseMapping(int x, int y)
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

int _level1::winMSG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    if(wParam == 'W'){
                mdl3D->pos.z -= .2f;
                //myCam->camMoveFdBd(1);
                //myCam->eye.z -=.2f;
                //myCam->des.z -=.2f;
                //myCam->setUpCamera();

                //mdl3D->actionTrigger = mdl3D->RUN;
            }
            else if(wParam == 'S'){
                mdl3D->pos.z += .2f;
                //myCam->eye.z +=.2f;
                //myCam->des.z +=.2f;
                //myCam->camMoveFdBd(-1);
                //myCam->setUpCamera();
                //mdl3D->actionTrigger = mdl3D->RUN;
            }
            else if(wParam == 'A'){
                mdl3D->pos.x += .2f;
                //myCam->camMoveLtRt(-1);
                //myCam->eye.x -=.2f;
                //myCam->des.x -=.2f;
                //myCam->setUpCamera();
                //mdl3D->actionTrigger = mdl3D->RUN;
            }
            else if(wParam == 'D'){
                mdl3D->pos.x -= .2f;
                //mdl3D->actionTrigger = mdl3D->RUN;
                //myCam->camMoveLtRt(1);
                //myCam->eye.x +=.2f;
                //myCam->des.x +=.2f;
                //myCam->setUpCamera();
            }
            if(mdl3D->pos.x < -10.0f){
                mdl3D->pos.x = -10.0f;
            }else if(mdl3D->pos.x > 10.0f){
                mdl3D->pos.x = 10.0f;
            }

            if(mdl3D->pos.z < -25.0f){
                mdl3D->pos.z = -25.0f;
            }else if(mdl3D->pos.z > 25.0f){
                mdl3D->pos.z = 25.0f;
            }

    switch(uMsg)
    {
        case WM_KEYDOWN:
            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            mdl3D->actionTrigger=mdl3D->STAND;
            mdl3DW->actionTrigger=mdl3DW->STAND;
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

        case WM_MOUSEMOVE:
              myInput->wParam = wParam;
              myInput->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
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
