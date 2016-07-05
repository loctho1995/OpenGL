#pragma once
#define DEGTORAD 0.0174533

class Helper3D
{
public:
    Helper3D();
    ~Helper3D();

    static void MatrixIdentity(float *pMatrix);

    static void MatrixUnit(float *pMatrix);

    static void MatrixZero(float *matrix);

    //a = matrix 4 x 4 - b = matrix 4 x 1, result matrix 4 x 1
    static void MatrixMul4(float *a, float *b, float *result);

    //result = a * b (a = matrix 4x4, b = matrix 4x4)
    static void MatrixMul16(float *a, float *b, float *result);

    //tinh tien ma tran a theo x, y ,z
    static void MatrixTranslate(float *a, float x, float y, float z);

    //scale ma tran theo x, y, z
    static void MatrixScale(float *a, float x, float y, float z);

    //quay 1 goc al pha quanh x, y, z
    static void MatrixRotation(float *a, float alpha, float x, float y, float z);

    //phep chieu song song
    static void MatrixOrtho(float *a, float left, float right, float bottom, float top, float near, float far);

    //phep chieu phoi canh
    static void MatrixFrustum(float *a, float left, float right, float bottom, float top, float near, float far);   
};

