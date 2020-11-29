//
// Created by Edwy Mandret on 29/05/2020.
//

#ifndef cube3_H
#define cube3_H

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __APPLE__
    // Defined before OpenGL and GLUT includes to avoid deprecation messages
    #define GL_SILENCE_DEPRECATION
    #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
    #include <GLFW/glfw3.h>
    #include <OpenGL/gl3.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#define DESIRED_FRAME_RATE 60
#define DESIRED_FRAME_TIME ((double) 1 / (double) DESIRED_FRAME_RATE)

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900

#define HORIZONTAL_DIV ((float) 1 / (float) WINDOW_WIDTH)
#define VERTICAL_DIV ((float) 1 / (float) WINDOW_HEIGHT)

#define MATRIX_IDENTITY                                                                                                \
    {                                                                                                                  \
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1                                                                 \
    }

// Homogeneous coordinates vector:
//
//  |x|
//  |y|
//  |z|
//  |w|
//

#pragma pack(push, 1)
typedef struct Vec4
{
    float x;
    float y;
    float z;
    float w;
} Vec4;
#pragma pack(pop)

// Homogeneous coordinates matrix:
//
//  |r11 r12 r13 tX|   |x|   |x.r11 + y.r12 + z.r13 + w.tX|
//  |r21 r22 r23 tY| * |y| = |x.r21 + y.r22 + z.r23 + w.tX|
//  |r31 r32 r33 tZ|   |z|   |x.r31 + y.r32 + z.r33 + w.tX|
//  |pX  pY  pZ  G |   |w|   |x.pX  + y.pY  + z.pZ  + w.G |
//
// The struct is packed such that the contiguous memory layout is:
//
//  |r11|r12|r13|tX|r21|r22|r23|tY|r31|r32|r33|tZ|pX|pY|pZ|G|
//
// Matrix uniforms are passed with the transpose matrix such that:
//
//                  T
//  |r11 r12 r13 tX|   |r11 r21 r31 pX|
//  |r21 r22 r23 tY| = |r12 r22 r32 pY|
//  |r31 r32 r33 tZ|   |r31 r32 r33 pZ|
//  |pX  pY  pZ  G |   |tX  tY  tZ  G |
//
// Otherwise the default contiguous memory should be:
//
//  |r11|r21|r31|pX|r12|r22|r32|pY|r13|r23|r33|pZ|tX|tY|tZ|G|
//

#pragma pack(push, 1)
typedef struct Matrix4
{
    float r11, r12, r13, tX;
    float r21, r22, r23, tY;
    float r31, r32, r33, tZ;
    float pX, pY, pZ, G;
} Matrix4;
#pragma pack(pop)

typedef struct Model
{
    Matrix4 orientation;
} Model;

// The global window object declared here
extern GLFWwindow *window;

// For importing shaders
unsigned int importFragmentShader(char const *pathname);
unsigned int importVertexShader(char const *pathname);
unsigned int linkShaders(unsigned int fragmentShader, unsigned int vertexShader);

#endif
