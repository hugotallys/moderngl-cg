//
//  Transformações I
//
//  Original code created by Marcelo Costa on 15/12/12.
//  Copyright (c) 2012 Universidade Federal de Alagoas - UFAL. All rights reserved.
//
//  Adapted by Hugo Oliveira on 16/08/25.
//

#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

typedef struct color_
{
    float r, g, b;
} Color;

typedef struct vertex_
{
    float x, y;
} Vertex;

static GLfloat currentAngle = 0.0f;
static GLfloat rotationSpeed = 1.0f;

void glutSetupWindow(const char* windowName, const int width, const int height)
{
    glutInitWindowSize(width, height);
    glutCreateWindow(windowName);

    glMatrixMode(GL_PROJECTION); // Projeção 2D
    gluOrtho2D(0.0, 600.0, 0.0, 400.0); // Definindo os limites do ViewPort
}

void rectangle(const Vertex center, const float width, const float height, const Color color)
{
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_POLYGON);
    glVertex2f(center.x - 0.5f * width, center.y - 0.5f * height);
    glVertex2f(center.x + 0.5f * width, center.y - 0.5f * height);
    glVertex2f(center.x + 0.5f * width, center.y + 0.5f * height);
    glVertex2f(center.x - 0.5f * width, center.y + 0.5f * height);
    glEnd();
}

void triangle(const Vertex anchor, const float angle, const Color color)
{
    glPushMatrix();
    glTranslatef(anchor.x, anchor.y, 0.0f);
    glRotatef(angle, 0.0f,0.0f,1.0f);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
    glVertex2f (0.0f, 0.0f);
    glVertex2f (-25.0f, 50.0f);
    glVertex2f (25.0f, 50.0f);
    glEnd();
    glPopMatrix();
}

void display()
{
    /* Limpar todos os píxeis */
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Cor de fundo preto

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const Vertex rodCenter = {320.f, 100.f };
    const Color rodColor = {1.0f, 1.0f, 0.0f };

    rectangle(rodCenter, 5.f, 200.f, rodColor);

    const Vertex bladeCenter = {320.0f, 200.f};
    const Color white = {1.0f, 1.0f, 1.0f};
    const Color red = {1.0f, 0.0f, 0.0f};
    const Color green = {0.0f, 1.0f, 0.0f};
    const Color blue = {0.0f, 0.0f, 1.0f};

    triangle(bladeCenter, currentAngle, white);
    triangle(bladeCenter, currentAngle + 90.0f, red);
    triangle(bladeCenter, currentAngle + 180.0f, green);
    triangle(bladeCenter, currentAngle + 270.0f, blue);

    /* Não esperar! */
    glFlush();
}

void animate()
{
    currentAngle += rotationSpeed;
    currentAngle = (currentAngle > 360.0f) ? 0.0f : currentAngle;
    glutPostRedisplay(); // Request window redraw
    glutTimerFunc(16, animate, 0); // Wait for 1 / 60 s
}

void handleSpecialKeys(const int key, int x, int y) {
    switch(key) {
    case GLUT_KEY_UP:
        rotationSpeed += 0.5f; // Increase speed
        break;
    case GLUT_KEY_DOWN:
        rotationSpeed -= 0.5f; // Decrease speed
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutSetupWindow("Cata-vento",640, 640);

    printf("*** Cata-vento - OpenGL ***\n");
    printf("Pressione SETA CIMA/BAIXO para controlar a velocidade de rotação das pás.\n");

	glutDisplayFunc(display);

    glutSpecialFunc(handleSpecialKeys);
    glutTimerFunc(16, animate, 0);

	glutMainLoop();

	return 0;
}