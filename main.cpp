#include <GL/glew.h>
#include <GLUT/glut.h>
#include <cstdio>

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("GLEW Test");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        // Handle the error appropriately.
        // ...
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Further code goes here...

    return 0;
}
