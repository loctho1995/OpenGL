#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
#include "Texture2D.h"
#include "GLENUM.h"

class Model
{
public:
    //tao moi 1 model 
    //can biet cac Vertex va so Vertex
    //indices can ve va so luong indices
    //SHADER_TYPE tuy thuoc loai shader ma load file shader tuong ung
    //texture path neu SHADER_TYPE can dung texture thi truyen texture vao
    static Model* createModel(int count, Vertex *vertices, int *indices, int numOfIndice, 
        SHADER_TYPE type = SHADER_POSITION_COLOR_TEXCOORD, const char* texturePath = "");

    //Model(int count, float *data);
    Model();
    ~Model();

    //lay Vertex Array Object ID
    //VAO nhu la dia chi tro den data cua cac buffer (VBO)
    GLuint getVAOID();

    //id cuar VBO chua data cua Vertices
    GLuint getVBOID();

    //lay so luong Vertex
    int getCount();

    //danh sach cac VAO
    std::vector<GLuint> getListVAOs();

    //danh sach cac VBO
    std::vector<GLuint> getListVBOs();

    //lay program cua Model
    //program nay chua cac file shader (fragment va vertex)
    GLuint getModelProgram();

    //lay texture cua model, moi model co luu tru 1 texture rieng
    Texture2D* getTexture();

    //set vitri cho texture (giong nhu translate di 1 doan x, y, z
    //dich chuyen theo local space 
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 pos);

    //by degree
    //xoay model 1 goc angle quanh truc axis
    void setRotation(float angle, glm::vec3 axis);
    void rotation(float angle, glm::vec3 axis);

    //set do co gian cho model
    void setScale(float scaleX, float scaleY, float scaleZ);
    void setScale(float scale);
    void scale(float x, float y, float z);
    void scale(float scale);

    glm::vec3 getPosition();

    //lay danh sach cac vertex
    Vertex *getVertices();

    //Lay matrix cua Model (o Local Space)
    glm::mat4 getModelMatrix();

    //so luong indices de cho biet ve nhung vertex nao
    int countIndices();

    void cleanUp();

protected:

    //dung cho ke thua giong nhu ham create
    bool init(int count, Vertex *vertices,int *indices, int numOfIndice, 
                        SHADER_TYPE type = SHADER_POSITION_COLOR_TEXCOORD, const char* texturePath = "");

    //tao cac Vertex Array Object
    GLuint createVAO(); //vertex array object

    //tao cac Vertex Buffer Object
    //o day can thong tin cua cac Vertex va indices cua no
    void createVBO(Vertex *vertices, int *indices); //vertex buffer object

    //tao shader program tuy thuoc vao SHADER_TYPE
    void createModelProgram();

    void unbindVAO();

    //update matrix cua Model, moi lan thay doi nhu setPosition, rotate hay scale thi se thay doi matrix nay
    void updateFinalMatrix();

    GLuint vaoID; //vertex array object id
    GLuint vboID; //vertex buffer object id
    GLuint modelProgram; 
    int count;    
    Texture2D *texture;

    SHADER_TYPE shaderType;

    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;

    glm::vec3 position;

    Vertex *vertices;

    int numOfIndice;

    //cac matrix tuong ung cua model
    glm::mat4 scaleMatrix, rotationMatrix, translateMatrix, finalMatrix;
};

