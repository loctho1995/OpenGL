#include <SOIL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define GLEW_STATIC
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h> // For wglSwapInterval
#endif

#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "Model.h"
#include "Renderer.h"
#include "Vertex.h"
#include "TimeCounter.h"
#include "Texture2D.h"
#include "Camera3D.h"
#include "Cube.h"

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

int winWidth = 800;
int winHeight = 600;
int g_iWindowHandle = 0;

int g_W, g_A, g_S, g_D, g_Q, g_E;
bool g_bShift = false;
bool g_bAnimate = true;


glm::ivec2 mousePos;
int        mouseButton;

float deltaTime = 0;

//Vertex vertex[] = {
//    Vertex(glm::vec3(-0.5f, -0.5f, 0.0), glm::vec2(0.0f, 1.0f)),
//    Vertex(glm::vec3(0.5f, -0.5f, 0.0), glm::vec2(1.0f, 1.0f)),
//    Vertex(glm::vec3(-0.5f, 0.5f, 0.0), glm::vec2(0.0f, 0.0f)),
//    Vertex(glm::vec3(0.5f, 0.5f, 0.0), glm::vec2(1.0f, 0.0f))
//};

Vertex coodinate[] = {
    //Origin
    Vertex(glm::vec3(0, 0, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),

    //Ox - Red        
    Vertex(glm::vec3(3.0f, 0, 0), glm::vec4(1.0, 0, 0, 1.0f)),

    //Oy - Green
    Vertex(glm::vec3(0.0, 3.0f, 0), glm::vec4(0, 1.0, 0, 1.0f)),

    //Oz - Blue
    Vertex(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec4(0, 0, 1.0, 1.0f))
};

int coordinateIndices[] = {
    0, 1, // Ox
    0, 2, // Oy
    0, 3 // Oz  
};

float off = 150.0f;


//Vertex box[6][4] = {
//    {
//        //face
//        //bot - left
//        Vertex(glm::vec3(-2.0f, -2.0f, 0.5f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(2.0f, -2.0f, 0.5f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(-2.0f, 2.0f, 0.5f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//        //top- right
//        Vertex(glm::vec3(2.0f, 2.0f, 0.5f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    },
//    {
//        //right
//        //bot - left
//        Vertex(glm::vec3(2.0f, -2.0f, -0.5f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(2.0f, -2.0f, -2.0f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(2.0f, 2.0f, -0.5f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//    },
//        //top - right
//        Vertex(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    {
//        //back
//        //bot - left
//        Vertex(glm::vec3(2.0f, -2.0f, -2.0f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//        //top - right
//        Vertex(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    },
//    {
//        //left
//        //bot - left
//        Vertex(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(-2.0f, -2.0f, -0.5f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//        //top - right
//        Vertex(glm::vec3(-2.0f, 2.0f, -0.5f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    },
//    {
//        //top
//        //bot - left
//        Vertex(glm::vec3(-2.0f, 2.0f, -0.5f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(2.0f, 2.0f, -0.5f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//        //top - right
//        Vertex(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    },
//    {
//        //under
//        //bot - left
//        Vertex(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec4(0.26f, 0.94f, 0.96, 1.0f), glm::vec2(0.0f, 1.0f)),
//        //bot - right
//        Vertex(glm::vec3(2.0f, -2.0f, -2.0f), glm::vec4(0.97f, 0.45f, 0.6, 1.0f), glm::vec2(1.0f, 1.0f)),
//        //top - left
//        Vertex(glm::vec3(-2.0f, -2.0f, -0.5f), glm::vec4(0.6f, 0.14f, 0.9, 1.0f), glm::vec2(0.0f, 0.0f)),
//        //top - right
//        Vertex(glm::vec3(2.0f, -2.0f, -0.5f), glm::vec4(0.36f, 0.92f, 0.15, 1.0f), glm::vec2(1.0f, 0.0f))
//    }
//};

void IdleGL();
void DisplayGL();
void KeyboardGL(unsigned char c, int x, int y);
void KeyboardUpGL(unsigned char c, int x, int y);
void SpecialGL(int key, int x, int y);
void SpecialUpGL(int key, int x, int y);
void MouseGL(int button, int state, int x, int y);
void MotionGL(int x, int y);
void ReshapeGL(int w, int h);

std::string format(const char* format, ...)
{
#define CC_MAX_STRING_LENGTH (1024 * 100)

    std::string ret;

    va_list ap;
    va_start(ap, format);

    char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
        ret = buf;
        free(buf);
    }

    va_end(ap);

    return ret;
}

void initGL(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    // Create an OpenGL 3.3 core forward compatible context.
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitWindowPosition((iScreenWidth - winWidth) / 2, (iScreenHeight - winHeight) / 2);
    glutInitWindowSize(winWidth, winHeight);

    g_iWindowHandle = glutCreateWindow("OpenGL Learning");

    // Register GLUT callbacks.
    glutIdleFunc(IdleGL);
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutKeyboardUpFunc(KeyboardUpGL);
    glutSpecialFunc(SpecialGL);
    glutSpecialUpFunc(SpecialUpGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(MotionGL);
    glutReshapeFunc(ReshapeGL);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_RASTERIZER_DISCARD);
    glEnable(GL_STENCIL_TEST);
    

    std::cout << "Initialize OpenGL Success!" << std::endl;
}

void InitGLEW()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "There was a problem initializing GLEW. Exiting..." << std::endl;
        exit(-1);
    }

    // Check for 3.3 support.
    // I've specified that a 3.3 forward-compatible context should be created.
    // so this parameter check should always pass if our context creation passed.
    // If we need access to deprecated features of OpenGL, we should check
    // the state of the GL_ARB_compatibility extension.
    if (!GLEW_VERSION_3_3)
    {
        std::cerr << "OpenGL 3.3 required version support not present." << std::endl;
        exit(-1);
    }

#ifdef _WIN32
    if (WGLEW_EXT_swap_control)
    {
        wglSwapIntervalEXT(0); // Disable vertical sync
    }
#endif
}

GLuint vao, MVP;
GLuint gProgram = 0;
GLuint vertexbuffer, indexBuffer, colorBuffer, texCoordBuffer;

std::vector<Model*> models;
std::vector<Texture2D*> textures;
Texture2D *texture;
Renderer *renderer;
Camera3D *camera, *staticCamera;
GLuint shaderProgram, uniformMVP;
Model *cord;
std::vector<Cube*> cubes;


void main(int argc, char* argv[])
{
    initGL(argc, argv);
    InitGLEW();

    cord = Model::createModel(sizeof(coodinate) / sizeof(Vertex), coodinate, coordinateIndices,
                                sizeof(coordinateIndices) / sizeof(int), SHADER_TYPE::SHADER_POSITION_COLOR);

    glm::vec3 pos = glm::vec3(0, 0, 0);
    Cube *cube = new Cube(pos, 1.0f, "textures/crate.png");
    //cube->setRotation(90, glm::vec3(0, 1.0f, 0));
    cube->setScale(0.8f);
    cubes.push_back(cube);

    renderer = new Renderer();
    camera = new Camera3D(glm::vec2(0, 0), winWidth, winHeight);
    staticCamera = new Camera3D(glm::vec2(0, 0), winWidth, winHeight);
    
    glm::mat4 project = glm::perspective(60.0f, (float)winWidth / winHeight, 1.0f, 5.0f);
    camera->setProject(project);

    glm::mat4 view = glm::lookAt(
        glm::vec3(2.0, 2.0, 2.0),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1.0, 0));

    camera->setView(view);

    staticCamera->setProject(project);
    staticCamera->setView(view);

    glutMainLoop();
}

float angle = 0;
float delta = 0;

void DisplayGL()
{
    renderer->preRender();

    deltaTime = TimeCounter::GetInstance()->GetCouter();
    angle += 0.08f;
    delta += 0.0001f;
    //std::cout << "fps: " << 1.0 / deltaTime << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //camera->setRotation(angle, glm::vec3(0, 0.0, 1.0));
    //camera->translate(glm::vec3(0, 0, -delta));

    //std::cout << delta << std::endl;

    glm::mat4 mvp = camera->getMVP();

    renderer->render(cord, mvp, GL_LINES);
    
    //cubes.at(0)->setRotation(angle, glm::vec3(0, 1.0, 0));
    //cubes.at(0)->scale(delta);
    //cubes.at(0)->setPosition(cubes.at(0)->getPosition() + glm::vec3(1, 0, 0));

    for (size_t i = 0; i < cubes.size(); i++)
    {
        auto models = cubes.at(i)->getModels();

        for (size_t j = 0; j < models.size(); j++)
        {
            renderer->render(models.at(j), mvp, GL_TRIANGLES);
        }
    }

    glutSwapBuffers();

    TimeCounter::GetInstance()->StartCounter();
}

void IdleGL()
{
    glutPostRedisplay();
}

void KeyboardGL(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 'w':
    case 'W':
        break;
    case 'a':
    case 'A':
        break;
    case 's':
    case 'S':
        break;
    case 'd':
    case 'D':
        break;
    }
}

void KeyboardUpGL(unsigned char c, int x, int y)
{

}

void SpecialGL(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
    {
        g_bShift = true;
    }
    break;
    }
}

void SpecialUpGL(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
    {
        g_bShift = false;
    }
    break;
    }
}

void MouseGL(int button, int state, int x, int y)
{
    mousePos = glm::ivec2(x, y);
    mouseButton = button;
}

float cameraSpeed = 0.5f;
float horizontalAngle, verticalAngle;

void MotionGL(int x, int y)
{
    glm::ivec2 pos = glm::ivec2(x, y);
    glm::vec2 delta = glm::vec2(pos - mousePos);
    mousePos = pos;



    //camera->rotation(sin(delta.y) * cos(delta.x) * cameraSpeed, glm::vec3(1.0, 0, 0));
    ////camera->rotation(sin(y) * cameraSpeed, glm::vec3(0, 1.0, 0));

    glm::vec3 right = glm::vec3(

        );
}

void ReshapeGL(int w, int h)
{
    glutPostRedisplay();
}

