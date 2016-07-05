#include <iostream>
#include "graphics.h"
#include "3DStruct.h"
#include "Graphics3D.h"
#include "TimeCounter.h"
#include <vector>

#pragma comment(lib, "graphics.lib")

float angle = 0;
Graphics3D *graphics, *graphicsOXYZ;


// 3D OBJECT    ///////////////////////////////////////////////
static ZVertex3D faceFront[] = {
    { -100, -100, 100 },
    { 100, -100, 100 },
    { 100, 100, 100 },
    { -100, 100, 100 },
};

static ZVertex3D faceBack[] = {
    { -100, -100, -100 },
    { 100, -100, -100 },
    { 100, 100, -100 },
    { -100, 100, -100 },
};

static ZVertex3D faceTop[] = {
    { 100, 100, 100 },
    { -100, 100, 100 },
    { -100, 100, -100 },
    { 100, 100, -100 },
};

static ZVertex3D faceBottom[] = {
    { 100, -100, 100 },
    { -100, -100, 100 },
    { -100, -100, -100 },
    { 100, -100, -100 },
};

static ZVertex3D Ox[] = {
    { 0, 0, 0 },
    { 150, 0, 0 },
};

static ZVertex3D Oy[] = {
    { 0, 0, 0 },
    { 0, 200, 0 },
};

static ZVertex3D Oz[] = {
    { 0, 0, 0 },
    { 0, 0, 100 },
};

static std::vector<ZVertex3D*> platForm;

void loop();

bool init()
{
    int width = getwindowwidth() / 4;
    int height = getwindowheight() / 4;

    float distance = 25.0f;
    int size = width / distance;
    
    for (int i = 0; i < size; i++)
    {
        ZVertex3D *vertex = new ZVertex3D[4];

        vertex[0].x = -width / 2 + i * distance;
        vertex[0].y = -100;
        vertex[0].z = -height;

        vertex[1].x = -width / 2 + i * distance;
        vertex[1].y = -100;
        vertex[1].z = height;

        vertex[2].x = -width / 2 + (i + 1) * distance;
        vertex[2].y = -100;
        vertex[2].z = height;

        vertex[3].x = -width / 2 + (i + 1) * distance;
        vertex[3].y = -100;
        vertex[3].z = -height;

        platForm.push_back(vertex);
    }

    return true;
}
void main()
{
    float delta = 0;
    float FPS = 1.0 / 60.0;

    int gd = DETECT, gm;

    initgraph(&gd, &gm, "c:\\tc\\bgi");

    graphics = new Graphics3D(new Graphics3D(), getwindowwidth(), getwindowheight());
    graphicsOXYZ = new Graphics3D(new Graphics3D(), getwindowwidth(), getwindowheight());

    //graphicsOXYZ->Rotate(60, 0, 1, 0);
    //graphics->Rotate(60, 0, 1, 0);

    init();

    while (true)
    {
        TimeCounter::GetInstance()->StartCounter();

        if (delta >= FPS)
        {
            cleardevice();

            loop();

            //std::cout << "delta time:  " << delta << std::endl;

            delta = 0;
        }

        delta += TimeCounter::GetInstance()->GetCouter();
    }

    getch();
    closegraph();
}

void loop()
{
    angle = 0.8;

    graphicsOXYZ->Rotate(angle, 1, 1, 1);
    graphics->Rotate(angle, 1, 1, 1);

    //mau cam
    setcolor(500);
    graphicsOXYZ->drawPolygon(Ox, 2);

    //cyan
    setcolor(65532);
    graphicsOXYZ->drawPolygon(Oy, 2);

    //mau vang 
    setcolor(65316);
    graphicsOXYZ->drawPolygon(Oz, 2);
    
    //graphics->Scale(0.5, 0.5, 0.5);
    //graphics->Translate(50, 20, 10);
    //mau trang
    setcolor(16777215);

    for (size_t i = 0; i < platForm.size(); i++)
    {
        auto vertex = platForm.at(i);

        graphics->drawPolygon(vertex, 4);

        
    }

    graphics->drawPolygon(faceFront, 4);
    graphics->drawPolygon(faceTop, 4);
    graphics->drawPolygon(faceBack, 4);
    graphics->drawPolygon(faceBottom, 4);
}