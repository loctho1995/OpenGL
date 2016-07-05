#include <ViewMatrixDemoPCH.h>
#include <Camera.h>

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;
int g_iWindowHandle = 0;

int g_W, g_A, g_S, g_D, g_Q, g_E;
bool g_bShift = false;
bool g_bAnimate = true;


glm::ivec2 g_MousePos;
int        g_MouseButton;
glm::quat g_Rotation;

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

Camera g_Camera;
glm::vec3 g_InitialCameraPosition;
glm::quat g_InitialCameraRotation;

float g_Pitch = 0.0f;
float g_Yaw = 0.0f;

struct VertexXYZColor
{
    glm::vec3 m_Pos;
    glm::vec3 m_Color;
};

// Define the 8 vertices of a unit cube
VertexXYZColor g_Vertices[8] = {
    { glm::vec3(  1,  1,  1 ), glm::vec3( 1, 1, 1 ) }, // 0
    { glm::vec3( -1,  1,  1 ), glm::vec3( 0, 1, 1 ) }, // 1
    { glm::vec3( -1, -1,  1 ), glm::vec3( 0, 0, 1 ) }, // 2
    { glm::vec3(  1, -1,  1 ), glm::vec3( 1, 0, 1 ) }, // 3
    { glm::vec3(  1, -1, -1 ), glm::vec3( 1, 0, 0 ) }, // 4
    { glm::vec3( -1, -1, -1 ), glm::vec3( 0, 0, 0 ) }, // 5
    { glm::vec3( -1,  1, -1 ), glm::vec3( 0, 1, 0 ) }, // 6
    { glm::vec3(  1,  1, -1 ), glm::vec3( 1, 1, 0 ) }, // 7
};

// Define the vertex indices for the cube.
// Each set of 6 vertices represents a set of triangles in 
// counter-clockwise winding order.
GLuint g_Indices[36] = {
    0, 1, 2, 2, 3, 0,           // Front face
    7, 4, 5, 5, 6, 7,           // Back face
    6, 5, 2, 2, 1, 6,           // Left face
    7, 0, 3, 3, 4, 7,           // Right face
    7, 6, 1, 1, 0, 7,           // Top face
    3, 2, 5, 5, 4, 3            // Bottom face
};

// Vertex array object for the cube.
GLuint g_vaoCube = 0;
GLuint g_ShaderProgram = 0;
// Model, View, Projection matrix uniform variable in shader program.
GLint g_uniformMVP = -1;

void IdleGL();
void DisplayGL();
void KeyboardGL( unsigned char c, int x, int y );
void KeyboardUpGL( unsigned char c, int x, int y );
void SpecialGL( int key, int x, int y );
void SpecialUpGL( int key, int x, int y );
void MouseGL( int button, int state, int x, int y );
void MotionGL( int x, int y );
void ReshapeGL( int w, int h );

/**
 * Initialize the OpenGL context and create a render window.
 */
void InitGL( int argc, char* argv[] )
{
    std::cout << "Initialize OpenGL..." << std::endl;

    glutInit( &argc, argv );

    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);

    // Create an OpenGL 3.3 core forward compatible context.
    glutInitContextVersion( 3, 3 );
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

    glutInitWindowPosition( ( iScreenWidth - g_iWindowWidth ) / 2, (iScreenHeight - g_iWindowHeight) / 2 );
    glutInitWindowSize( g_iWindowWidth, g_iWindowHeight );

    g_iWindowHandle = glutCreateWindow("OpenGL Template");

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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::cout << "Initialize OpenGL Success!" << std::endl;
}

void InitGLEW()
{
    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK )
    {
        std::cerr << "There was a problem initializing GLEW. Exiting..." << std::endl;
        exit(-1);
    }

    // Check for 3.3 support.
    // I've specified that a 3.3 forward-compatible context should be created.
    // so this parameter check should always pass if our context creation passed.
    // If we need access to deprecated features of OpenGL, we should check
    // the state of the GL_ARB_compatibility extension.
    if ( !GLEW_VERSION_3_3 )
    {
        std::cerr << "OpenGL 3.3 required version support not present." << std::endl;
        exit(-1);
    }

#ifdef _WIN32
    if ( WGLEW_EXT_swap_control )
    {
        wglSwapIntervalEXT(0); // Disable vertical sync
    }
#endif
}

// Loads a shader and returns the compiled shader object.
// If the shader source file could not be opened or compiling the 
// shader fails, then this function returns 0.
GLuint LoadShader( GLenum shaderType, const std::string& shaderFile )
{
    std::ifstream ifs;

    // Load the shader.
    ifs.open(shaderFile);

    if ( !ifs )
    {
        std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
        return 0;
    }

    std::string source( std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) );
    ifs.close();

    // Create a shader object.
    GLuint shader = glCreateShader( shaderType );

    // Load the shader source for each shader object.
    const GLchar* sources[] = { source.c_str() };
    glShaderSource( shader, 1, sources, NULL );

    // Compile the shader.
    glCompileShader( shader );

    // Check for errors
    GLint compileStatus;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus ); 
    if ( compileStatus != GL_TRUE )
    {
        GLint logLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
        GLchar* infoLog = new GLchar[logLength];
        glGetShaderInfoLog( shader, logLength, NULL, infoLog );

#ifdef _WIN32
        OutputDebugString(infoLog);
#else
        std::cerr << infoLog << std::endl;
#endif
        delete infoLog;
        return 0;
    }

    return shader;
}

// Create a shader program from a set of compiled shader objects.
GLuint CreateShaderProgram( std::vector<GLuint> shaders )
{
    // Create a shader program.
    GLuint program = glCreateProgram();

    // Attach the appropriate shader objects.
    for( GLuint shader: shaders )
    {
        glAttachShader( program, shader );
    }

    // Link the program
    glLinkProgram(program);

    // Check the link status.
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus );
    if ( linkStatus != GL_TRUE )
    {
        GLint logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        GLchar* infoLog = new GLchar[logLength];

        glGetProgramInfoLog( program, logLength, NULL, infoLog );

#ifdef _WIN32
        OutputDebugString(infoLog);
#else
        std::cerr << infoLog << std::endl;
#endif

        delete infoLog;
        return 0;
    }

    return program;
}

int main( int argc, char* argv[] )
{
    std::cout << "===[Controls]==================================================================" << std::endl;
    std::cout << "Press [SPACE] to toggle the animation." << std::endl;
    std::cout << "Hold [SHIFT] to move the camera faster." << std::endl;
    std::cout << "Press [W] to move the camera forward." << std::endl;
    std::cout << "Press [A] to move the camera left." << std::endl;
    std::cout << "Press [S] to move the camera backward." << std::endl;
    std::cout << "Press [D] to move the camera right." << std::endl;
    std::cout << "Press [Q] to move the camera upwards." << std::endl;
    std::cout << "Press [E] to move the camera downwards." << std::endl;
    std::cout << "Press [LEFT MOUSE BUTTON] to rotate the camera." << std::endl;
    std::cout << "Press [RIGHT MOUSE BUTTON] to rotate the geometry." << std::endl;
    std::cout << "Press [R] to reset the camera to it's default position and orientation." << std::endl;
    std::cout << "Press [ESC] to end the program." << std::endl;
    std::cout << "===============================================================================" << std::endl;

    g_PreviousTicks = std::clock();
    g_A = g_W = g_S = g_D = g_Q = g_E = 0;

    g_InitialCameraPosition = glm::vec3( 0, 0, 10 );
    g_Camera.SetPosition( g_InitialCameraPosition );
    g_Camera.SetRotation( g_InitialCameraRotation );

    InitGL(argc, argv);
    InitGLEW();

    // Load some shaders.
    GLuint vertexShader = LoadShader( GL_VERTEX_SHADER, "../data/shaders/simpleShader.vert" );
    GLuint fragmentShader = LoadShader( GL_FRAGMENT_SHADER, "../data/shaders/simpleShader.frag" );

    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    // Create the shader program.
    g_ShaderProgram = CreateShaderProgram( shaders );
    assert( g_ShaderProgram != 0 );

    GLint positionAtribID = glGetAttribLocation( g_ShaderProgram, "in_position" );
    GLint colorAtribID = glGetAttribLocation( g_ShaderProgram, "in_color" );
    g_uniformMVP = glGetUniformLocation( g_ShaderProgram, "MVP" );

    // Create a VAO for the cube.
    glGenVertexArrays( 1, &g_vaoCube );
    glBindVertexArray( g_vaoCube );

    GLuint vertexBuffer, indexBuffer;
    glGenBuffers( 1, &vertexBuffer );
    glGenBuffers( 1, &indexBuffer );

    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(g_Indices), g_Indices, GL_STATIC_DRAW );

    glVertexAttribPointer( positionAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor,m_Pos) );
    glEnableVertexAttribArray( positionAtribID );

    glVertexAttribPointer( colorAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor,m_Color) );
    glEnableVertexAttribArray( colorAtribID );

    // Make sure we disable and unbind everything to prevent rendering issues later.
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glDisableVertexAttribArray( positionAtribID );
    glDisableVertexAttribArray( colorAtribID );

    glutMainLoop();
}

void ReshapeGL( int w, int h )
{
    if ( h == 0 )
    {
        h = 1;
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    g_Camera.SetViewport( 0, 0, w, h );
    g_Camera.SetProjectionRH( 60.0f, w/(float)h, 0.1f, 100.0f );

    glutPostRedisplay();
}

glm::mat4 LookAtRH( glm::vec3 eye, glm::vec3 target, glm::vec3 up )
{
    using namespace glm;
    vec3 zaxis = normalize(eye - target);    // The "forward" vector.
    vec3 xaxis = normalize(cross(up, zaxis));// The "right" vector.
    vec3 yaxis = cross(zaxis, xaxis);     // The "up" vector.

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    mat4 viewMatrix(
        vec4(       xaxis.x,            yaxis.x,            zaxis.x,       0 ),
        vec4(       xaxis.y,            yaxis.y,            zaxis.y,       0 ),
        vec4(       xaxis.z,            yaxis.z,            zaxis.z,       0 ),
        vec4( -dot( xaxis, eye ), -dot( yaxis, eye ), -dot( zaxis, eye ),  1 )
    );

    return viewMatrix;

    mat4 orientation(
        vec4( xaxis.x, yaxis.x, zaxis.x, 0 ),
        vec4( xaxis.y, yaxis.y, zaxis.y, 0 ),
        vec4( xaxis.z, yaxis.z, zaxis.z, 0 ),
        vec4(   0,       0,       0,     1 )
        );
    // Create a 4x4 translation matrix.
    // The eye position is negated which is equivalent
    // to the inverse of the translation matrix. 
    // T(v)^-1 == T(-v)
    mat4 translation(
        vec4(   1,      0,      0,   0 ),
        vec4(   0,      1,      0,   0 ), 
        vec4(   0,      0,      1,   0 ),
        vec4(-eye.x, -eye.y, -eye.z, 1 )
        );

    // Combine the orientation and translation to compute 
    // the final view matrix
    return ( orientation * translation );
}

glm::mat4 FPSViewRH( glm::vec3 eye, float yaw, float pitch )
{
    using namespace glm;

    // If the pitch and yaw angles are in degrees,
    // they need to be converted to radians. Here
    // I assume the values are already converted to radians.
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    vec3 xaxis( cosYaw, 0, -sinYaw );
    vec3 yaxis( sinYaw * sinPitch, cosPitch, cosYaw * sinPitch );
    vec3 zaxis( sinYaw * cosPitch, -sinPitch, cosYaw * cosPitch );

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    mat4 viewMatrix(
        vec4(       xaxis.x,            yaxis.x,            zaxis.x,      0 ),
        vec4(       xaxis.y,            yaxis.y,            zaxis.y,      0 ),
        vec4(       xaxis.z,            yaxis.z,            zaxis.z,      0 ),
        vec4( -dot( xaxis, eye ), -dot( yaxis, eye ), -dot( zaxis, eye ), 1 )
        );

    //return viewMatrix;

    mat4 rotX = eulerAngleX( pitch );
    mat4 rotY = eulerAngleY( yaw );
    //mat4 rotation = rotY * rotX;
    mat4 rotation = eulerAngleYX( yaw, pitch );
    mat4 translation = translate(eye);

    return inverse( translation * rotation );
}

void DisplayGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBindVertexArray( g_vaoCube );
    glUseProgram( g_ShaderProgram );

    glm::mat4 projectionMatrix = g_Camera.GetProjectionMatrix();
    glm::mat4 viewMatrix = g_Camera.GetViewMatrix();
    //glm::mat4 viewMatrix = LookAtRH( g_Camera.GetPosition(), glm::vec3(0), glm::vec3(0, 1, 0 ) );
    //glm::mat4 viewMatrix = FPSViewRH( g_Camera.GetPosition(), glm::radians(g_Yaw), glm::radians(g_Pitch) );
    glm::mat4 vp = projectionMatrix * viewMatrix;

    static float totalTime = 0.0f;
    static std::clock_t previousTime = std::clock();
    std::clock_t currentTime = std::clock();
    float deltaTime = ( currentTime - previousTime ) / (float)CLOCKS_PER_SEC;
    previousTime = currentTime;

    if ( g_bAnimate )
    {
        totalTime += deltaTime;
    }

    glm::mat4 rotation = glm::toMat4(g_Rotation);

    for ( int i = -5; i <= 5; ++i )
    {
        for ( int j = -5; j <= 5; ++j )
        {
            for ( int k = -5; k <= 5; ++k )
            {
                glm::mat4 mvp = vp * ( glm::translate( glm::vec3( i * 10, j * 10, k * 10 ) ) * rotation * glm::scale( glm::vec3( cos(totalTime) + 2, sin(totalTime) + 2, 1) ) );
                glUniformMatrix4fv( g_uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp) );
                glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
            }
        }
    }

    glUseProgram(0);
    glBindVertexArray(0);

    glutSwapBuffers();
}

void IdleGL()
{
    g_CurrentTicks = std::clock();
    float deltaTicks = (float)( g_CurrentTicks - g_PreviousTicks );
    g_PreviousTicks = g_CurrentTicks;

    float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

    float cameraSpeed = 1.0f;
    if ( g_bShift )
    {
        cameraSpeed = 5.0f;
    }

    g_Camera.Translate( glm::vec3( g_D - g_A, g_Q - g_E, g_S - g_W ) * cameraSpeed * fDeltaTime );

    glutPostRedisplay();
}

void KeyboardGL( unsigned char c, int x, int y )
{
    switch ( c )
    {
    case 'w':
    case 'W':
        g_W = 1;
        break;
    case 'a':
    case 'A':
        g_A = 1;
        break;
    case 's':
    case 'S':
        g_S = 1;
        break;
    case 'd':
    case 'D':
        g_D = 1;
        break;
    case 'q':
    case 'Q':
        g_Q = 1;
        break;
    case 'e':
    case 'E':
        g_E = 1;
        break;
    case 'r':
    case 'R':
        g_Camera.SetPosition( g_InitialCameraPosition );
        g_Camera.SetRotation( g_InitialCameraRotation );
        g_Rotation = glm::quat();
        g_Pitch = g_Yaw = 0.0f;
        break;
    case ' ':
        g_bAnimate = !g_bAnimate;
        break;
    case 27:
        glutLeaveMainLoop();
        break;
    }
}

void KeyboardUpGL( unsigned char c, int x, int y )
{
    switch ( c )
    {
    case 'w':
    case 'W':
        g_W = 0;
        break;
    case 'a':
    case 'A':
        g_A = 0;
        break;
    case 's':
    case 'S':
        g_S = 0;
        break;
    case 'd':
    case 'D':
        g_D = 0;
        break;
    case 'q':
    case 'Q':
        g_Q = 0;
        break;
    case 'e':
    case 'E':
        g_E = 0;
        break;

    default:
        break;
    }
}

void SpecialGL( int key, int x, int y )
{
    switch( key )
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
        {
            g_bShift = true;
        }
        break;
    }
}

void SpecialUpGL( int key, int x, int y )
{
    switch( key )
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
        {
            g_bShift = false;
        }
        break;
    }
}

void MouseGL( int button, int state, int x, int y )
{
    g_MousePos = glm::ivec2(x,y);
    g_MouseButton = button;
}

void MotionGL( int x, int y )
{
    glm::ivec2 mousePos = glm::ivec2( x, y );
    glm::vec2 delta = glm::vec2( mousePos - g_MousePos );
    g_MousePos = mousePos;

    glm::quat rotX = glm::angleAxis<float>( glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0) );
    glm::quat rotY = glm::angleAxis<float>( glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0) );

    //    g_Camera.Rotate( rotX * rotY );

    if ( g_MouseButton == GLUT_LEFT_BUTTON )
    {
        g_Pitch += delta.y * 0.5f;
        g_Yaw += delta.x * 0.5f;

        g_Pitch = glm::clamp( g_Pitch, -80.0f, 80.0f );
        g_Yaw = fmod(g_Yaw, 360.0f);

        glm::quat cameraRotation = glm::toQuat(glm::eulerAngleYX( glm::radians(g_Yaw), glm::radians(g_Pitch) ));
        g_Camera.SetRotation(cameraRotation);
    }
    else
    {
        g_Rotation = ( rotX * rotY ) * g_Rotation;
    }
}

