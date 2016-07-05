#include "Texture2D.h"
#include <SOIL.h>

Texture2D::Texture2D()
{
}


Texture2D::~Texture2D()
{
}

Texture2D* Texture2D::createTexture(const char* filePath)
{
    Texture2D* texture = new Texture2D();

    if (texture && texture->init(filePath))
    {
        return texture;
    }

    delete texture;
    texture = nullptr;

    return nullptr;
}

GLuint Texture2D::getTextureID()
{
    return textureID;
}

bool Texture2D::init(const char* filePath)
{
    GLuint vbo;

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE20, 0);
    
    return true;
}

int Texture2D::getWidth()
{
    return width;
}

int Texture2D::getHeight()
{
    return height;
}