#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "GLENUM.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void preRender(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

    void render(Model *model, glm::mat4 &mvp, GLenum type = GL_TRIANGLES);
};