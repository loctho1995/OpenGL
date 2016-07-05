#pragma once

#include "3DStruct.h"
#include "graphics.h"
#include "Helper3D.h"
#include <iostream>
#include <string>

class Graphics3D
{
public:
    Graphics3D();
    Graphics3D(Graphics3D *graphics, int width, int height);
    ~Graphics3D();

    //void clearScreen();
    void drawPolygon(ZVertex3D *vertex, int count);
    //void swapBuffer();

    //module bien doi
    void loadIndentity(int index);
    void Rotate(float anpha, float x, float y, float z);
    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Projection(float *vertexIn, float *vertexOut);
    void updateFinalMatrix();
    //module ma tran
    //void setViewPort(float x, float y, float width, float height);
    //void getMatrix(float *m, int index);
    //void setMatrix(float *m, int index);
    void transform(float *in, float *out);

protected:
    //ma tran bien doi (xoay, tinh tien, scale,...)
    float *modelViewMatrix;

    //ma tran chieu
    float *projectionMatrix;

    //ma tran khung nhin
    float *viewPortMatrix;

    //ma tran cuoi cung
    float *projectedMatrix;

    Graphics3D *render;
    Graphics3D *buffer;
};

