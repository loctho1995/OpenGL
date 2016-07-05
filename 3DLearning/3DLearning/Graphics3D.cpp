#include "Graphics3D.h"


Graphics3D::Graphics3D()
{
}

Graphics3D::Graphics3D(Graphics3D * graphics, int width, int height)
{
    modelViewMatrix = new float[16];
    projectionMatrix = new float[16];
    projectedMatrix = new float[16];

    Helper3D::MatrixUnit(modelViewMatrix);
    Helper3D::MatrixUnit(projectionMatrix);
    Helper3D::MatrixUnit(projectedMatrix);
    Helper3D::MatrixFrustum(projectionMatrix, 0, 0, 0, 0, 15, 0);

    viewPortMatrix = new float[4];

    viewPortMatrix[0] = 0;
    viewPortMatrix[1] = 0;
    viewPortMatrix[2] = (float)width;
    viewPortMatrix[3] = (float)height;

    //Doi tuong ve ra cua so
    render = graphics;

    //khoi tao double buffer
    //buffer = render->
}

Graphics3D::~Graphics3D()
{
}

void Graphics3D::Rotate(float alpha, float x, float y, float z)
{
    Helper3D::MatrixRotation(modelViewMatrix, alpha, x, y, z);

    updateFinalMatrix();
}

void Graphics3D::Translate(float x, float y, float z)
{
    Helper3D::MatrixTranslate(modelViewMatrix, x, y, z);

    updateFinalMatrix();
}

void Graphics3D::Scale(float x, float y, float z)
{
    Helper3D::MatrixScale(modelViewMatrix, x, y, z);

    updateFinalMatrix();
}

void Graphics3D::updateFinalMatrix()
{
    float *matrix1 = new float[16];
    Helper3D::MatrixMul16(viewPortMatrix, projectionMatrix, matrix1);
    Helper3D::MatrixMul16(modelViewMatrix, matrix1, projectedMatrix);
}

void Graphics3D::drawPolygon(ZVertex3D *v, int count)
{
    float *vertex3D = new float[4];
    int i;

    float *aTransformVertex = new float[count * 4];
    float *aProjectionVertex = new float[count * 4];

    float *q;
    float *p = aTransformVertex;

    for (size_t i = 0; i < count; i++)
    {
        vertex3D[0] = v[i].x;
        vertex3D[1] = v[i].y;
        vertex3D[2] = v[i].z;
        vertex3D[3] = 1.0;

        //bien doi quay tinh tien
        this->transform(vertex3D, p);

        p += 4;
    }

    //chieu vertex (loai bo truc z)
    p = aProjectionVertex;
    q = aTransformVertex;

    int countx = 0;

    for (size_t i = 0; i < count; i++)
    {
        Projection(q, p);

        //*(p + 1) = - *(p + 1);
        if (p[3] != 0)
        {
            *(p) /= *(p + 3); //x / w
            *(p + 1) /= *(p + 3); //y / w
            *(p + 2) /= *(p + 3); // z / w
        }

        //std::cout << "a [" << countx << "] = " << "[ " << p[0] << ", " << p[1] << ", " << p[2] << ", " << p[3] << std::endl;
        //chuyen ve goc toa do
        *(p) += *(viewPortMatrix) + *(viewPortMatrix + 2) / 2.0;
        *(p + 1) += *(viewPortMatrix) + *(viewPortMatrix + 3) / 2.0;

        p += 4;
        q += 4;
        countx++;
    }

    //ve dagiac 2D
    p = aProjectionVertex;
    q = aTransformVertex;

    int x0, y0, x, y;

    for (size_t i = 0; i < count; i++)
    {
        //ve duong thang
        if (i != 0)
        {
            line(x, y, p[0], p[1]);
        }
        else
        {
            //diem dau tien luu x0, y0
            x0 = (int)p[0];
            y0 = (int)p[1];
        }

        //luu toa do xy
        x = (int)p[0];
        y = (int)p[1];

        p += 4;
    }

    if (count > 1)
    {
        line(x0, y0, x, y);
    }

    delete aProjectionVertex;
    delete aTransformVertex;
}

void Graphics3D::transform(float *in, float *out)
{
    Helper3D::MatrixMul4(modelViewMatrix, in, out);
}

void Graphics3D::Projection(float *vertexIn, float *vertexOut)
{
    Helper3D::MatrixMul4(projectionMatrix, vertexIn, vertexOut);
}