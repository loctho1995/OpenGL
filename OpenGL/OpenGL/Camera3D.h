#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera3D
{
public:
    Camera3D(glm::vec2 position, int width, int height);
    ~Camera3D();

    void translate(glm::vec3 position);

    //angle in degree
    void rotation(float angle, glm::vec3 axis);

    void setViewPort(glm::vec2 position, int width, int height);

    void setView(glm::mat4 view);

    void scale(float x = 1, float y = 1, float z = 1);

    void setProjectPerspective(float angle, float screenRatio, float near, float far);

    void setProject(glm::mat4 projection);

    //angle in degree
    void setRotation(float angle, glm::vec3 axis);

    void setTranslate(glm::vec3 position);

    glm::mat4 getMVP();
    
    glm::vec2 position;
    int width, height;

protected:
    glm::mat4 view, model, projection, scaleMat, rotationMat, translateMat;
    glm::mat4 viewPort;
};

