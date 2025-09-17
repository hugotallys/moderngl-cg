#ifndef BALL_H
#define BALL_H

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>

class Ball {
private:
    float x, y, z;
    float vx, vy, vz;
    float radius;

public:
    Ball(float startX = 0.0f, float startY = 0.0f, float startZ = 1.0f, float r = 1.0f);

    void update(float fieldWidth, float fieldHeight);
    void render();
    void reset();
    void applyForce(float fx, float fy, float fz);

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getRadius() const { return radius; }

    // Setters
    void setPosition(float newX, float newY, float newZ);
    void setVelocity(float newVx, float newVy, float newVz);

    bool checkGoal(float fieldHeight, float goalWidth, float goalHeight);
};

#endif
