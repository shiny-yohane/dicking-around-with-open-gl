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
    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    printf("sizeof(vertices): %lu\n", sizeof(vertices));
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int width = 600;
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
    glPointSize(8.0f);

    createVertexBuffer();

    display();
    glutDisplayFunc(render);

    glutMainLoop();

    return 0;
}
