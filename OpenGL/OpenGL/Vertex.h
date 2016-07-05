#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex
{
public:
    glm::vec3 pos;
    glm::vec4 color;
    glm::vec2 texCoord;
    //glm::vec3 normal;

    Vertex(glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2 texCoord = glm::vec2(0.0f, 0.0f), 
            glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    Vertex::Vertex(glm::vec3 pos, glm::vec4 color);
    ~Vertex();
};

