#include "Helper3D.h"
#include <iostream>

Helper3D::Helper3D()
{
}


Helper3D::~Helper3D()
{
}

void Helper3D::MatrixUnit(float *matrix)
{
    for (size_t i = 0; i < 15; i++)
    {
        *(matrix + i) = 0;
    }

    *(matrix) = 1;
    *(matrix + 5) = 1;
    *(matrix + 10) = 1;
    *(matrix + 15) = 1;
}

void Helper3D::MatrixZero(float *matrix)
{
    for (size_t i = 0; i < 15; i++)
    {
        *(matrix + i) = 0;
    }
}

void Helper3D::MatrixIdentity(float *matrix)
{
    //dich sang 2 bit = x4
#define M(m, row, col) m[(col << 2) + row]

    std::memset(matrix, 0, 16 * sizeof(float));

    M(matrix, 0, 0) = 1;
    M(matrix, 1, 1) = 1;
    M(matrix, 2, 2) = 1;
    M(matrix, 3, 3) = 1; 
    M(matrix, 4, 4) = 1;

    #undef M
}

void Helper3D::MatrixMul4(float *a, float *b, float *result)
{
    #define mA(row, col) *(a + (col << 2) + row)

    result[0] = mA(0, 0)*b[0] + mA(0, 1)*b[1] + mA(0, 2)*b[2] + mA(0, 3)*b[3];
    result[1] = mA(1, 0)*b[0] + mA(1, 1)*b[1] + mA(1, 2)*b[2] + mA(1, 3)*b[3];
    result[2] = mA(2, 0)*b[0] + mA(2, 1)*b[1] + mA(2, 2)*b[2] + mA(2, 3)*b[3];
    result[3] = mA(3, 0)*b[0] + mA(3, 1)*b[1] + mA(3, 2)*b[2] + mA(3, 3)*b[3];

    #undef mA
}

void Helper3D::MatrixMul16(float *a, float *b, float *result)
{

#define mR(row, col) *(result + (col << 2) + row)
#define mA(row, col) *(a + (col << 2) + row)
#define mB(row, col) *(b + (col << 2) + row)


    int i;
    for (i = 0; i < 4; i++)
    {
        mR(i, 0) = mA(i, 0) * mB(0, 0) + mA(i, 1) * mB(1, 0) + mA(i, 2) * mB(2, 0) + mA(i, 3) * mB(3, 0);
        mR(i, 1) = mA(i, 0) * mB(0, 1) + mA(i, 1) * mB(1, 1) + mA(i, 2) * mB(2, 1) + mA(i, 3) * mB(3, 1);
        mR(i, 2) = mA(i, 0) * mB(0, 2) + mA(i, 1) * mB(1, 2) + mA(i, 2) * mB(2, 2) + mA(i, 3) * mB(3, 2);
        mR(i, 3) = mA(i, 0) * mB(0, 3) + mA(i, 1) * mB(1, 3) + mA(i, 2) * mB(2, 3) + mA(i, 3) * mB(3, 3);
    }

#undef mR
#undef mA
#undef mB
}

void Helper3D::MatrixTranslate(float *a, float x, float y, float z)
{
    float *TranslateMatrix = new float[16];
    float Result[16];

    MatrixUnit(TranslateMatrix);

    TranslateMatrix[3] = x;
    TranslateMatrix[7] = y;
    TranslateMatrix[11] = z;

    MatrixMul16(a, TranslateMatrix, Result);

    memcpy(a, Result, sizeof(Result));
}

void Helper3D::MatrixScale(float *a, float x, float y, float z)
{
    float *ScaleMatrix = new float[16];
    float Result[16];

    MatrixUnit(ScaleMatrix);

    ScaleMatrix[0] = x;
    ScaleMatrix[5] = y;
    ScaleMatrix[10] = z;

    MatrixMul16(a, ScaleMatrix, Result);

    memcpy(a, Result, sizeof(Result));
}

void Helper3D::MatrixRotation(float *a, float alpha, float x, float y, float z)
{
    float *RotationMatrix = new float[16];
    float *Result = new float[16];

    MatrixZero(Result);
    MatrixUnit(RotationMatrix);

    //khong xoay
    if (x == y && y == x && z == y && z == 0)
    {
        return;
    }

    #define M(m, col, row) *(m + (col << 2) + row)

    float s = (float)sin(alpha * DEGTORAD);
    float c = (float)cos(alpha * DEGTORAD);

    // Xoay quanh truc Z
    //  Cos  Sin  0   0
    // -Sin  Cos  0   0
    //   0    0   1   0
    //   0    0   0   1

    if (x == 0 && y == 0)
    {
        M(RotationMatrix, 0, 0) = c;
        M(RotationMatrix, 0, 1) = s;
        M(RotationMatrix, 1, 0) = -s;
        M(RotationMatrix, 1, 1) = c;
    }

    // Xoay quanh truc Y
    //  Cos   0 -Sin   0
    //   0    1   0    0
    //  Sin   0  Cos   0
    //   0    0   0    1
    else if (x == 0 && z == 0)
    {
        M(RotationMatrix, 0, 0) = c;
        M(RotationMatrix, 0, 2) = s;
        M(RotationMatrix, 2, 0) = -s;
        M(RotationMatrix, 2, 2) = c;
    }

    // Xoay quanh truc X
    //   1   0     0   0
    //   0  Cos   Sin  0
    //   0 -Sin   Cos  0
    //   0   0     0   1
    else if (y == 0 && z == 0)
    {
        M(RotationMatrix, 1, 1) = c;
        M(RotationMatrix, 1, 2) = -s;
        M(RotationMatrix, 2, 1) = s;
        M(RotationMatrix, 2, 2) = c;
    }

    // Xoay tren ca 3 truc (cong thuc da duoc chung minh nhu sau)
    else
    {
        /*
        [txx + c    txy – sz     txz + sy   0]
        [txy + sz   tyy + c      tyz – sx   0]
        [txz + sy   tyz + sx     tzz + c    0]
        [    0          0            0      1]

        c = cos(a)
        s = sin(a)
        t = 1 – cos(a)
        x,y,z = Vector Unit
        */
        // Chuyen vector(x,y,z) thanh vector don vi
        float d = (float)sqrt(x*x + y*y + z*z);

        // Qua nho -> khong xoay ma tran
        if (d <= 1.0e-4)
            return;

        x = x / d;
        y = y / d;
        z = z / d;

        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        float one_c = (float)1.0 - c;

        M(RotationMatrix, 0, 0) = (one_c * xx) + c;
        M(RotationMatrix, 0, 1) = (one_c * xy) - zs;
        M(RotationMatrix, 0, 2) = (one_c * zx) + ys;

        M(RotationMatrix, 1, 0) = (one_c * xy) + zs;
        M(RotationMatrix, 1, 1) = (one_c * yy) + c;
        M(RotationMatrix, 1, 2) = (one_c * yz) - xs;

        M(RotationMatrix, 2, 0) = (one_c * zx) - ys;
        M(RotationMatrix, 2, 1) = (one_c * yz) + xs;
        M(RotationMatrix, 2, 2) = (one_c * zz) + c;
    }

    MatrixMul16(a, RotationMatrix, Result);

    //copy to model matrix
    for (size_t i = 0; i < 15; i++)
    {
        *(a + i) = *(Result + i);
    }

    #undef M
}

void Helper3D::MatrixOrtho(float *a, float left, float right, float bottom, float top, float near, float far)
{
    /*
    1    0    0     0
    0    1    0     0
    0    0    0     0
    0    0   near   1

    [x y z 1] => [x y near 1]
    */
    float *OrthorMatrix = new float[16];
    float *Result = new float[16];

    MatrixUnit(OrthorMatrix);
    MatrixUnit(Result);

    *(OrthorMatrix + 10) = 0;
    *(OrthorMatrix + 11) = near;

    MatrixMul16(a, OrthorMatrix, Result);

    for (size_t i = 0; i < 16; i++)
    {
        *(a + i) = *(Result + i);
    }
}

void Helper3D::MatrixFrustum(float *a, float left, float right, float bottom, float top, float near, float far)
{
    /*
    near     0      0           0
    0       near    0           0
    0       0       0           1/near
    0       0       near*near   0

    [x y z 1] => [near near near*near z / near]
    */

    float *FrustumMatrix = new float[16];
    float *Result = new float[16];

    MatrixIdentity(FrustumMatrix);

    //chieu
    //*(FrustumMatrix + 11) = 1.0f / near;  
    //*(FrustumMatrix + 14) = near * near;
    *(FrustumMatrix + 15) = 1;

    //ty le
    *(FrustumMatrix + 0) = 1;
    *(FrustumMatrix + 5) = 1;
    *(FrustumMatrix + 10) = 1;

    MatrixMul16(a, FrustumMatrix, Result);

    for (size_t i = 0; i < 16; i++)
    {
        *(a + i) = *(Result + i);
    }
}
