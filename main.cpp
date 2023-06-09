#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>

GLuint VBO;

void display()
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
    GLfloat vertices[] = {0.0f, 0.0f, 0.0f};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0); // which is different from glEnableVertexArrayAttrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_ARRAY_BUFFER, 0, 1);
    glDisableVertexAttribArray(0);

    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int width = 800;
    int height = 600;
    glutInitWindowSize(width, height);

    int x = 200;
    int y = 200;
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

    GLfloat red = 0.0f, green = 0.0f, blue = 0.0f, alpha = 0.0f;
    glClearColor(red, green, blue, alpha);

    createVertexBuffer();

    display();
    glutDisplayFunc(render);

    glutMainLoop();

    return 0;
}
