#include "Cube.h"

Cube::Cube(glm::vec3 position, float width, const char* filePath)
{
    Vertex box[] = {
        //face
        //bot - left
        Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        //top- right
        Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
        
        //right
        //bot - left
        Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        //top - right
        Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),

        //back
        //bot - left
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        //top - right
        Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),

        //left
        //bot - left
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        //top - right
        Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),

        //top
        //bot - left
        Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        //top - right
        Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
        
        //under
        //bot - left
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
        //bot - right
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
        //top - left
        Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        //top - right
        Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f))
    };

    int indices[] = {
        //face
        //phai ve theo chieu kim dong ho cua vertex
        0, 1, 2,
        2, 1, 3,

        //right
        4, 5, 6,
        6, 5, 7,

        //back
        8, 9, 10,
        10, 9, 11,

        //left
        12, 13, 14,
        14, 13, 15,

        //top
        16, 17, 18,
        18, 17, 19,

        //under
        20, 21, 22,
        22, 21, 23
    };

    //for (size_t i = 0; i < 6; i++)
    //{
    //    for (size_t j = 0; j < 4; j++)
    //    {
    //        auto p = box[i][j].pos;
    //        box[i][j].pos = glm::vec3(p.x, p.y, p.z);
    //        box[i][j].pos += position;
    //    }
    //}

    Model *model = Model::createModel(sizeof(box) / sizeof(Vertex), box, indices, sizeof(indices) / sizeof(int) ,
                                            SHADER_TYPE::SHADER_POSITION_COLOR_TEXCOORD, filePath);
    models.push_back(model);

    this->setPosition(position);
}

Cube::~Cube()
{

}

void Cube::setPosition(glm::vec3 position)
{
    this->position = position;

    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->setPosition(position);
    }
}

void Cube::setRotation(float angle, glm::vec3 axis)
{
    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->setRotation(angle, axis);
    }
}

void Cube::rotation(float angle, glm::vec3 axis)
{
    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->rotation(angle, axis);
    }
}

void Cube::setScale(float scale)
{
    this->setScale(scale, scale, scale);
}

void Cube::setScale(float x, float y, float z)
{
    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->setScale(x, y, z);
    }
}

std::vector<Model*> Cube::getModels()
{
    return models;
}

glm::vec3 Cube::getPosition()
{
    return position;
}

void Cube::scale(float x, float y, float z)
{
    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->scale(x, y, z);
    }
}

void Cube::scale(float scale)
{
    this->scale(scale, scale, scale);
}