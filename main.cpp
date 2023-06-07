#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>

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

static void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay(); // what is this?
    glutSwapBuffers();
}

int main(int argc, char **argv) // why ** ?
{
    glutInit(&argc, argv); // yeah why & here?
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

    GLclampf Red = 1.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f; // GLclampf?
    glClearColor(Red, Green, Blue, Alpha);

    glutDisplayFunc(Render);

    glutMainLoop();

    return 0;
}
