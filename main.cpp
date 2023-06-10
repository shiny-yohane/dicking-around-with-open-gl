#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

GLuint VBO;

void displayVersion()
{
    const GLubyte *version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", version);
}

bool readFile(const char *fileName, std::string &outFile)
{
    std::ifstream file(fileName);
    if (!file)
    {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    outFile = buffer.str();

    return true;
}

void createVertexBuffer()
{
    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    printf("sizeof(vertices): %lu\n", sizeof(vertices));
}

void addShader(GLuint shaderProgram, const char *pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);
    if (shaderObj == 0)
    {
        fprintf(stderr, "Error creating shader type %d\n", shaderType);
        std::exit(1);
    }

    const GLchar *p[1];
    p[0] = pShaderText;

    GLint lengths[1];
    lengths[0] = strlen(pShaderText);

    glShaderSource(shaderObj, 1, p, lengths);
    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
        std::exit(1);
    }

    glAttachShader(shaderProgram, shaderObj);
}

void compileShaders()
{
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        std::exit(1);
    }

    std::string vs, fs;

    if (!readFile("shader.vert", vs))
    {
        std::exit(1);
    }
    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!readFile("shader.frag", fs))
    {
        std::exit(1);
    }
    addShader(shaderProgram, vs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(shaderProgram);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f};

    GLfloat *p = vertices;

    // GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
    //                       1.0f, -1.0f, 0.0f,
    //                       0.0f, 1.0f, 0.0f};

    GLfloat colors[] = {1.0f, 0.0f, 0.0f,  // Red color for vertex 1
                        0.0f, 1.0f, 0.0f,  // Green color for vertex 2
                        0.0f, 0.0f, 1.0f}; // Blue color for vertex 3

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        printf("You pressed the key 'Q'\n");
    }
}

int main(int argc, char **argv)
{

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        // Keep running
        glClear(GL_COLOR_BUFFER_BIT);

        // Render a triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.6f, -0.4f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.6f, -0.4f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.0f, 0.6f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
