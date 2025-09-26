#include "SoccerField.h"

SoccerField::SoccerField(
        const float w, const float h, const float gw, const float gh
    )
    : width(w), height(h), goalWidth(gw), goalHeight(gh) {}

void SoccerField::render() const {
    drawFieldSurface();
    drawFieldLines();
}

void SoccerField::renderGoals() const {
    // Goal 1 (top)
    drawGoal(height/2);
    // Goal 2 (bottom)
    drawGoal(-height/2);
}

void SoccerField::drawFieldSurface() const {
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

void SoccerField::drawFieldLines() const {
    glColor3f(1.0f, 1.0f, 1.0f); // White lines
    glLineWidth(4.0f);
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
        const float angle1 = static_cast<float>(i * M_PI) / 180.0f;
        const float angle2 = static_cast<float>((i + 10) * M_PI) / 180.0f;
        glVertex3f(cos(angle1) * 9.15f, sin(angle1) * 9.15f, 0.01f);
        glVertex3f(cos(angle2) * 9.15f, sin(angle2) * 9.15f, 0.01f);
    }

    // Penalty areas
    float areaWidth = 40.32f;
    float areaHeight = 16.5f;

    // Penalty area 1 (top)
    glVertex3f(-areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2, 0.01f);
    glVertex3f(areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2 - areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, height / 2, 0.01f);

    // Penalty area 2 (bottom)
    glVertex3f(-areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2 + areaHeight, 0.01f);
    glVertex3f(-areaWidth / 2, -height / 2, 0.01f);

    // Penalty Arcs

    const float penaltyArcYTop = height / 2 - areaHeight;
    const float penaltyArcYBottom = -height / 2 + areaHeight;

    // First arc (top)
    for (int i = 180; i < 360; i += 10)
    {
        const float angle1 = static_cast<float>(i * M_PI) / 180.0f;
        const float angle2 = static_cast<float>((i + 10) * M_PI) / 180.0f;
        glVertex3f(cos(angle1) * 9.15f, penaltyArcYTop + sin(angle1) * 9.15f, 0.01f);
        glVertex3f(cos(angle2) * 9.15f, penaltyArcYTop + sin(angle2) * 9.15f, 0.01f);
    }

    // Second arc (bottom)
    for (int i = 0; i < 180; i += 10)
    {
        const float angle1 = static_cast<float>(i * M_PI) / 180.0f;
        const float angle2 = static_cast<float>((i + 10) * M_PI) / 180.0f;
        glVertex3f(cos(angle1) * 9.15f, penaltyArcYBottom + sin(angle1) * 9.15f, 0.01f);
        glVertex3f(cos(angle2) * 9.15f, penaltyArcYBottom + sin(angle2) * 9.15f, 0.01f);
    }

    // Goal areas

    const float goalAreaWidth = 18.32f;
    const float goalAreaHeight = 5.5f;

    // Goal area 1 (top)
    glVertex3f(-goalAreaWidth / 2, height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, height / 2 - goalAreaHeight, 0.01f);
    glVertex3f(goalAreaWidth / 2, height / 2 - goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, height / 2 - goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, height / 2 - goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, height / 2, 0.01f);

    // Goal area 2 (bottom)
    glVertex3f(-goalAreaWidth / 2, -height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, -height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, -height / 2, 0.01f);
    glVertex3f(goalAreaWidth / 2, -height / 2 + goalAreaHeight, 0.01f);
    glVertex3f(goalAreaWidth / 2, -height / 2 + goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, -height / 2 + goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, -height / 2 + goalAreaHeight, 0.01f);
    glVertex3f(-goalAreaWidth / 2, -height / 2, 0.01f);

    glEnd();
}

void SoccerField::drawGoal(const float yAnchor) const {
    glColor3f(1.0f, 1.0f, 1.0f); // White posts
    glLineWidth(10.0f);

    // Left post
    glBegin(GL_LINES);
    glVertex3f(-goalWidth / 2, yAnchor, 0.0f);
    glVertex3f(-goalWidth / 2, yAnchor, goalHeight);
    // Right post
    glVertex3f(goalWidth / 2, yAnchor, 0.0f);
    glVertex3f(goalWidth / 2, yAnchor, goalHeight);
    // Crossbar
    glVertex3f(-goalWidth / 2, yAnchor, goalHeight);
    glVertex3f(goalWidth / 2, yAnchor, goalHeight);
    glEnd();
}
