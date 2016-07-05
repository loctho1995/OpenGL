#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Texture2D.h"
#include "Model.h"
#include "GLENUM.h"
#include <vector>

class Cube
{
public:
    Cube(glm::vec3 position, float width, const char* filePath = "");

    void setPosition(glm::vec3 position);
    
    void setRotation(float angle, glm::vec3 axis);

    void setScale(float x, float y, float z);

    void setScale(float scale);

    void rotation(float angle, glm::vec3 axis);

    void scale(float x, float y, float z);
    void scale(float scale);

    glm::vec3 getPosition();

    std::vector<Model*> getModels();

    ~Cube();

private:
    std::vector<Model*> models;

    glm::vec3 position;
};

