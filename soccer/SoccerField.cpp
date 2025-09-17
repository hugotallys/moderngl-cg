#include "SoccerField.h"

SoccerField::SoccerField(float w, float h, float gw, float gh, float gd)
    : width(w), height(h), goalWidth(gw), goalHeight(gh), goalDepth(gd) {}

void SoccerField::render() {
    drawFieldSurface();
    drawFieldLines();
}

void SoccerField::renderGoals() {
    // Goal 1 (top)
    drawGoal(0, height / 2 + goalDepth / 2, 0);
    // Goal 2 (bottom)
    drawGoal(0, -height / 2 - goalDepth / 2, 0);
}

void SoccerField::drawFieldSurface() {
    glColor3f(0.0f, 0.8f, 0.0f); // Green field
    glBegin(GL_TRIANGLES);
    // First triangle
    glVertex3f(-width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, height / 2.0f, 0.0f);

    // Second triangle
    glVertex3f(-width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, height / 2.0f, 0.0f);
    glVertex3f(-width / 2.0f, height / 2.0f, 0.0f);
    glEnd();
}

void SoccerField::drawFieldLines() {
    glColor3f(1.0f, 1.0f, 1.0f); // White lines
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // Field borders
    glVertex3f(-width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, height / 2, 0.01f);
    glVertex3f(width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, -height / 2, 0.01f);

    // Center line
    glVertex3f(-width / 2, 0, 0.01f);
    glVertex3f(width / 2, 0, 0.01f);

    // Center circle
    for (int i = 0; i < 360; i += 10) {
        float angle1 = i * M_PI / 180.0f;
        float angle2 = (i + 10) * M_PI / 180.0f;
        glVertex3f(cos(angle1) * 9.15f, sin(angle1) * 9.15f, 0.01f);
        glVertex3f(cos(angle2) * 9.15f, sin(angle2) * 9.15f, 0.01f);
    }

    // Goal areas
    float areaWidth = 16.5f;
    float areaHeight = 40.32f;

    // Goal area 1 (top)
    glVertex3f(-areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2, 0.01f);

    // Goal area 2 (bottom)
    glVertex3f(-areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2, 0.01f);

    glEnd();
}

void SoccerField::drawGoal(float x, float y, float z) {
    glColor3f(1.0f, 1.0f, 1.0f); // White posts

    // Left post
    glPushMatrix();
    glTranslatef(x - goalWidth / 2, y, z + goalHeight / 2);
    glScalef(0.5f, 0.5f, goalHeight);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Right post
    glPushMatrix();
    glTranslatef(x + goalWidth / 2, y, z + goalHeight / 2);
    glScalef(0.5f, 0.5f, goalHeight);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Crossbar
    glPushMatrix();
    glTranslatef(x, y, z + goalHeight);
    glScalef(goalWidth, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Net (simplified)
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (int i = 0; i <= 10; i++) {
        float pos = x - goalWidth / 2 + (goalWidth * i / 10.0f);
        glVertex3f(pos, y - goalDepth, z);
        glVertex3f(pos, y - goalDepth, z + goalHeight);
    }
    for (int i = 0; i <= 10; i++) {
        float pos = z + (goalHeight * i / 10.0f);
        glVertex3f(x - goalWidth / 2, y - goalDepth, pos);
        glVertex3f(x + goalWidth / 2, y - goalDepth, pos);
    }
    glEnd();
}
