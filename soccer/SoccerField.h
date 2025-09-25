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

public:
    SoccerField(float w = 68.0f, float h = 105.0f, float gw = 15.0f, float gh = 10.00f);

    void render() const;
    void renderGoals() const;

    // Getters
    [[nodiscard]] float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getGoalWidth() const { return goalWidth; }
    float getGoalHeight() const { return goalHeight; }

private:
    void drawFieldSurface() const;
    void drawFieldLines() const;
    void drawGoal(float yAnchor) const;
};

#endif
