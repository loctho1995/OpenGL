#include "Model.h"
#include <iostream>
#include "ShaderManager.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Model::Model()
{

}

Model::~Model()
{
}

Model* Model::createModel(int count, Vertex *vertices, int *indices, int numOfIndice, SHADER_TYPE type, const char* texturePath)
{
    Model *model = new Model();

    if (model && model->init(count, vertices, indices, numOfIndice, type, texturePath))
    {
        return model;
    }
    else
    {
        delete model;
        model = nullptr;
        return nullptr;
    }
}

bool Model::init(int count, Vertex *vertices,int *indices, int numOfIndice, SHADER_TYPE type, const char* texturePath)
{
    this->vertices = vertices;

    this->count = count;

    this->shaderType = type;

    this->numOfIndice = numOfIndice;

    this->texture = Texture2D::createTexture(texturePath);

    createModelProgram();

    glUseProgram(0);

    this->vaoID = createVAO();

    createVBO(vertices, indices);

    unbindVAO();

    this->setPosition(glm::vec3(0, 0, 0));

    return true;
}

GLuint Model::getModelProgram()
{
    return modelProgram;
}

GLuint Model::getVAOID()
{
    return vaoID;
}

GLuint Model::getVBOID()
{
    return vboID;
}

Texture2D* Model::getTexture()
{
    return texture;
}

int Model::getCount()
{
    return count;
}

int Model::countIndices()
{
    return numOfIndice;
}

GLuint Model::createVAO()
{
    GLuint vaoid;

    //tao va bind vertex array vao video card khi nao dung thi lay id VAO ra de su dung.
    //VAO giong nhu la ten dai dien cho vung nho cua VBO
    glGenVertexArrays(1, &vaoid);

    glBindVertexArray(vaoid);

    VAOs.push_back(vaoid);

    return vaoid;
}

void Model::createVBO(Vertex *vertices, int *indices)
{
    //vao moi va su dung buffer tren video card, thong tin ve cac attribute se duoc luu tru o day
    glGenBuffers(1, &vboID); //generate vbo id, buffer from Graphics Card

    VBOs.push_back(vboID);

    //bind data vao buffer (buffer vung nho o tren video card)
    glBindBuffer(GL_ARRAY_BUFFER, vboID); // bind vbo id
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, vertices, GL_STATIC_DRAW); //send data to buffer
    
    //pointer vao position cua shader
    glVertexAttribPointer(GL_SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(GL_SHADER_POSITION);

    //pointer vao color cua shader
    glVertexAttribPointer(GL_SHADER_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(GL_SHADER_COLOR);

    //pointer vao texcoord cua shader
    glVertexAttribPointer(GL_SHADER_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(GL_SHADER_TEXTCOORD);

    //glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

    //tao buffer chua indices
    GLuint vboIndices;
    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * numOfIndice, indices, GL_STATIC_DRAW);
    std::cout << "num of indices: " << sizeof(indices) * numOfIndice << std::endl;
    VBOs.push_back(vboIndices);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind vbo
}

void Model::createModelProgram()
{
    GLuint vshader;
    GLuint fshader;

    switch (shaderType)
    {
        case SHADER_POSITION_COLOR:
            vshader = ShaderManager::getInstance()->loadShader(GL_VERTEX_SHADER, "shaders/pos_color/simpleShader.vert");
            fshader = ShaderManager::getInstance()->loadShader(GL_FRAGMENT_SHADER, "shaders/pos_color/simpleShader.frag");
            break;

        case SHADER_POSITION_COLOR_TEXCOORD:
            vshader = ShaderManager::getInstance()->loadShader(GL_VERTEX_SHADER, "shaders/pos_color_tex/simpleShader.vert");
            fshader = ShaderManager::getInstance()->loadShader(GL_FRAGMENT_SHADER, "shaders/pos_color_tex/simpleShader.frag");
            break;

        default:
            break;
    }
    

    std::vector<GLuint> shaders;
    shaders.push_back(vshader);
    shaders.push_back(fshader);

    modelProgram = ShaderManager::getInstance()->loadShaderProgram(shaders);
}

void Model::unbindVAO()
{
    glBindVertexArray(0);
}

std::vector<GLuint> Model::getListVAOs()
{
    return VAOs;
}

std::vector<GLuint> Model::getListVBOs()
{
    return VBOs;
}

void Model::cleanUp()
{
    for (size_t i = 0; i < VAOs.size(); i++)
    {
        glDeleteVertexArrays(1, &VAOs.at(i));
    }

    for (size_t i = 0; i < VBOs.size(); i++)
    {
        glDeleteBuffers(1, &VBOs.at(i));
    }
}

void Model::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}

void Model::setPosition(glm::vec3 pos)
{
    this->position = pos;

    translateMatrix = glm::mat4();
    translateMatrix = glm::translate(translateMatrix, pos);

    updateFinalMatrix();
}

glm::vec3 Model::getPosition()
{
    return position;
}

Vertex* Model::getVertices()
{
    return vertices;
}

//by degree
void Model::setRotation(float angle, glm::vec3 axis)
{
    rotationMatrix = glm::mat4();
    rotationMatrix = glm::rotate(rotationMatrix, angle, axis);

    updateFinalMatrix();
}

void Model::rotation(float angle, glm::vec3 axis)
{
    rotationMatrix = glm::rotate(rotationMatrix, angle, axis);

    updateFinalMatrix();
}

void Model::setScale(float scale)
{    
    this->setScale(scale, scale, scale);

}
void Model::setScale(float scaleX, float scaleY, float scaleZ)
{
    scaleMatrix = glm::mat4();

    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scaleX, scaleY, scaleZ));

    updateFinalMatrix();
}

void Model::scale(float scale)
{
    this->scale(scale, scale, scale);
}

void Model::scale(float x, float y, float z)
{
    scaleMatrix = glm::mat4();
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(x, y, z));

    updateFinalMatrix();
}

glm::mat4 Model::getModelMatrix()
{
    return finalMatrix;
}

void Model::updateFinalMatrix()
{
    finalMatrix = translateMatrix * rotationMatrix * scaleMatrix;
}