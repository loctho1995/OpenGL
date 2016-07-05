#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::preRender(float red, float green, float blue , float alpha)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Model *model, glm::mat4 &mvp, GLenum type)
{
    glm::mat4 finalMVP = model->getModelMatrix() * mvp;

    glUseProgram(model->getModelProgram());

    glBindVertexArray(model->getVAOID());

    glActiveTexture(GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, model->getTexture()->getTextureID());

    glUniformMatrix4fv(GL_SHADER_MVP, 1, GL_FALSE, glm::value_ptr(finalMVP));

    //glEnableVertexAttribArray(GL_SHADER_POSITION);

    glDrawElements(type, model->countIndices(), GL_UNSIGNED_INT, 0);  
    //glDrawArrays(type, 0, model->getCount());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glBindVertexArray(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}