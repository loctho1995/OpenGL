#include "Vertex.h"


Vertex::Vertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec4 color)
{
    this->pos = pos;
    this->color = color;
    this->texCoord = texCoord;
}

Vertex::Vertex(glm::vec3 pos, glm::vec4 color)
{
    this->pos = pos;
    this->color = color;
}

Vertex::~Vertex()
{
}