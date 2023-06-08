#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>

void display()
{
    const GLubyte *version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", version);
}

void closeCallback()
{
    // Perform any necessary cleanup or application-specific actions
    // before closing the window
    // ...

    // Exit the program or close the window
    // exit(0); // or use glutDestroyWindow(windowId) if you want to close only the current window
    glutDestroyWindow(glutGetWindow());
    std::exit(0);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
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

    GLfloat red = 1.0f, green = 0.0f, blue = 0.0f, alpha = 0.0f;
    glClearColor(red, green, blue, alpha);

    display();
    glutDisplayFunc(render);

    glutMainLoop();

    return 0;
}
