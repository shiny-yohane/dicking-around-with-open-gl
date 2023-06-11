#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

GLuint VBO;
GLuint VAO;
GLuint gTranslationLocation;

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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

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

    gTranslationLocation = glGetUniformLocation(shaderProgram, "gTranslation");
    if (gTranslationLocation == -1)
    {
        fprintf(stderr, "Error getting uniform location of 'gTranslation'\n");
    std:
        exit(1);
    }

    // glValidateProgram(shaderProgram);
    // glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    // if (!Success)
    // {
    //     glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    //     fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    //     exit(1);
    // }

    glUseProgram(shaderProgram);
}

void render(GLFWwindow *window)
{
    static float scale = 0.0f;
    static float delta = 0.005f;

    scale += delta;
    if ((scale >= 1.0f) || (scale <= -1.0f))
    {
        delta *= -1.0f;
    }

    glm::mat4 translation(1.0f, 0.0f, 0.0f, scale * 2,
                          0.0f, 1.0f, 0.0f, scale,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glUniformMatrix4fv(gTranslationLocation, 1, GL_TRUE, &translation[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; // Enable experimental features
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        printf("GLEW initialisation failed: %s", glewGetErrorString(err));
        return -1;
    }

    displayVersion();

    createVertexBuffer();
    compileShaders();

    while (!glfwWindowShouldClose(window))
    {
        render(window);
    }

    return 0;
}
