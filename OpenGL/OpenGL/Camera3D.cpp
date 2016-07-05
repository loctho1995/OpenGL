#include "Camera3D.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Camera3D::Camera3D(glm::vec2 position, int width, int height)
{
    this->position = position;
    this->width = width;
    this->height = height;

    //viewPort = glm::mat4();
    //view = glm::mat4();

    //viewPort[0].x = 1.0f / width;
    //viewPort[1].y = 1.0f / height;
    //viewPort[3].x = position.x / ((float)width / 2);
    //viewPort[3].y = position.y / ((float)height / 2); 

    this->projection = glm::mat4();
    this->view = glm::mat4();
    this->model = glm::mat4();
    this->scaleMat = glm::mat4();
    this->translateMat = glm::mat4();
    this->rotationMat = glm::mat4();
}

Camera3D::~Camera3D()
{
}

void Camera3D::setViewPort(glm::vec2 position, int width, int height)
{
    this->position = position;
    this->width = width;
    this->height = height;
}

void Camera3D::setProjectPerspective(float angle, float screenRatio, float near, float far)
{
    projection = glm::perspective(glm::radians(angle), screenRatio, near, far);
}

void Camera3D::translate(glm::vec3 position)
{
    translateMat = glm::translate(translateMat, position);
}

void Camera3D::scale(float x , float y , float z )
{
    scaleMat = glm::scale(scaleMat, glm::vec3(x, y, z));
}

void Camera3D::rotation(float angle, glm::vec3 axis)
{
    rotationMat = glm::rotate(rotationMat, angle, axis);
}

void Camera3D::setRotation(float angle, glm::vec3 axis)
{
    rotationMat = glm::mat4();
    rotationMat = glm::rotate(rotationMat, angle, axis);
}

void Camera3D::setView(glm::mat4 view)
{
    this->view = view;
}

void Camera3D::setProject(glm::mat4 projection)
{
    this->projection = projection;
}

void Camera3D::setTranslate(glm::vec3 position)
{
    translateMat = glm::mat4();
    translate(position);
}

glm::mat4 Camera3D::getMVP()
{
    //this->projection[0][0] = 1.0f;
    //this->projection[1][1] = 1.0f;
    //this->projection[2][2] = 1.0f;
    //this->projection[2][3] = 0.0f;
    //this->projection[3][2] = 0.0f;
    //this->projection[3][3] = 3.0f;

    //projection = glm::mat4();
    return this->projection * view * translateMat * rotationMat * scaleMat * model;
}