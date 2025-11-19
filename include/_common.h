#ifndef _COMMON_H
#define _COMMON_H

#include<iostream>
#include<windows.h>
#include<string>

#include<gl/gl.h>
#include<GL/glut.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#include<fstream>

#define PI 3.14159

using namespace std;

typedef struct{
          float x;
          float y;
          } vec2;

typedef struct{
          float x;
          float y;
          float z;
          } vec3;

typedef struct{
          float r;
          float g;
          float b;
          float a;
          } col4;
enum Scene {
            MAIN,
            LEVEL1,
            LEVEL2,
            LEVEL3,
            CREDITS,
            HELP,
            QUIT
        };

#endif // _COMMON_H
