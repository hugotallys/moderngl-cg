#ifndef SOCCERFIELD_H
#define SOCCERFIELD_H

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>

class SoccerField {
private:
    float width;
    float height;
    float goalWidth;
    float goalHeight;
    float goalDepth;

public:
    SoccerField(float w = 68.0f, float h = 105.0f, float gw = 7.32f, float gh = 2.44f, float gd = 2.0f);

    void render();
    void renderGoals();

    // Getters
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getGoalWidth() const { return goalWidth; }
    float getGoalHeight() const { return goalHeight; }
    float getGoalDepth() const { return goalDepth; }

private:
    void drawFieldSurface();
    void drawFieldLines();
    void drawGoal(float x, float y, float z);
};

#endif
