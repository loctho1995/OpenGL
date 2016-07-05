#pragma once

void MatrixIdentity(float *matrix);

void MatrixMul4(float *a, float *b, float *result);

void MatrixMul16(float *a, float *b, float *result);

void MatrixTranslate(float *a, float x, float y, float z);

void MatrixScale(float *a, float x, float y, float z);

void MatrixRotation(float *a, float alpha, float x, float y, float z);