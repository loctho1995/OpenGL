#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture2D
{
public:
    static Texture2D *createTexture(const char* filePath);

    GLuint getTextureID();

    int getWidth();
    int getHeight();

    Texture2D();
    ~Texture2D();

protected:

    bool init(const char* filePath);

    GLuint textureID;

    int width, height;

    unsigned char* image;
};

