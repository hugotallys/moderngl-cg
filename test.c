/*
* Created by Hugo Tallys on 15/08/25.
* Desenhar catavento e fazer as pás girarem(controlar a velocidade)
*
*
*/

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct vertex
{
    float x, y, z;
    float r, g, b;
} Vertex;


Vertex vertices[] = {
    -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Red vertex
     0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Green vertex
     0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f  // Blue vertex
};

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(0.4f, 0.4f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.4f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < sizeof(vertices); i++)
    {
        const Vertex v = vertices[i];
        glColor3f(v.r, v.g, v.b);
        glVertex3f(v.x, v.y, v.z);
    }

    glEnd(); // Done drawing
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("OpenGL");

    // Register Callback functions
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
