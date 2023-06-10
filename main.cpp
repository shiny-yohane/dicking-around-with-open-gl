#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>

GLuint VBO;

void displayVersion()
{
    const GLubyte *version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", version);
}

void closeCallback()
{
    glutDestroyWindow(glutGetWindow());
    std::exit(0);
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

void compileShaders()
{
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        std::exit(0);
    }
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

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int width = 800;
    int height = 800;
    glutInitWindowSize(width, height);

    int x = 400;
    int y = 400;
    glutInitWindowPosition(x, y);

    int win = glutCreateWindow("GLEW Test");
    printf("window id: %d\n", win);

    glutWMCloseFunc(closeCallback);

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glPointSize(8.0f);

    // createVertexBuffer();
    compileShaders();

    displayVersion();
    glutDisplayFunc(render);

    glutMainLoop();

    return 0;
}
