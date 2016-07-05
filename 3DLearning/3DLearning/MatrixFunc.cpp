

#include <iostream>
#include "MatrixFunc.h"

#define DEGTORAD 0.0174533

void MatrixIdentity(float *matrix)
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

void MatrixMul4(float *a, float *b, float *result)
{
#define mA(row, col) a[(col << 2) + row]

    result[0] = mA(0, 0)*b[0] + mA(0, 1)*b[1] + mA(0, 2)*b[2] + mA(0, 3)*b[3];
    result[1] = mA(1, 0)*b[0] + mA(1, 1)*b[1] + mA(1, 2)*b[2] + mA(1, 3)*b[3];
    result[2] = mA(2, 0)*b[0] + mA(2, 1)*b[1] + mA(2, 2)*b[2] + mA(2, 3)*b[3];
    result[3] = mA(3, 0)*b[0] + mA(3, 1)*b[1] + mA(3, 2)*b[2] + mA(3, 3)*b[3];

#undef mA
}

void MatrixMul16(float *a, float *b, float *result)
{

#define mR(row, col) result[(col << 2) + row]
#define mA(row, col) a[(col << 2) + row]
#define mB(row, col) b[(col << 2) + row]


    int i;
    for (i = 0; i < 4; i++)
    {
        float x1 = mR(i, 0) = mA(i, 0) * mB(0, 0) + mA(i, 1) * mB(1, 0) + mA(i, 2) * mB(2, 0) + mA(i, 3) * mB(3, 0);
        float x2 = mR(i, 1) = mA(i, 0) * mB(0, 1) + mA(i, 1) * mB(1, 1) + mA(i, 2) * mB(2, 1) + mA(i, 3) * mB(3, 1);
        float x3 = mR(i, 2) = mA(i, 0) * mB(0, 2) + mA(i, 1) * mB(1, 2) + mA(i, 2) * mB(2, 2) + mA(i, 3) * mB(3, 2);
        float x4 = mR(i, 3) = mA(i, 0) * mB(0, 3) + mA(i, 1) * mB(1, 3) + mA(i, 2) * mB(2, 3) + mA(i, 3) * mB(3, 3);
    }

#undef mR
#undef mA
#undef mB
}

void MatrixTranslate(float *a, float x, float y, float z)
{
    float TranslateMatrix[16];
    float Result[16];

    MatrixIdentity(TranslateMatrix);

    TranslateMatrix[3] = x;
    TranslateMatrix[7] = y;
    TranslateMatrix[11] = z;

    MatrixMul16(a, TranslateMatrix, Result);

    memcpy(a, Result, sizeof(Result));
}

void MatrixScale(float *a, float x, float y, float z)
{
    float ScaleMatrix[16];
    float Result[16];

    MatrixIdentity(ScaleMatrix);

    ScaleMatrix[0] = x;
    ScaleMatrix[5] = y;
    ScaleMatrix[10] = z;

    MatrixMul16(a, ScaleMatrix, Result);

    memcpy(a, Result, sizeof(Result));
}

void MatrixRotation(float *a, float alpha, float x, float y, float z)
{
    float RotationMatrix[16];
    float Result[16];

    MatrixIdentity(RotationMatrix);

    //khong xoay
    if (x == y && y == x && z == y && z == 0)
    {
        return;
    }

#define M(m, col, row) m[(col << 2) + row]

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
        M(RotationMatrix, 0, 2) = -s;
        M(RotationMatrix, 2, 0) = s;
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
        M(RotationMatrix, 1, 2) = s;
        M(RotationMatrix, 2, 1) = -s;
        M(RotationMatrix, 2, 2) = c;
    }

    // Xoay tren ca 3 truc (cong thuc da duoc chung minh nhu sau)
    else
    {
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

    memcpy((void*)a, (void*)Result, sizeof(RotationMatrix));

#undef M
}