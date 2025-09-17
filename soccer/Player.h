#ifndef PLAYER_H
#define PLAYER_H

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#include <vector>
#include "Ball.h"

struct PlayerSegment {
    float x, y, z;
    float vx, vy, vz;
    float radius;
};

class Player {
private:
    std::vector<PlayerSegment> segments;
    int keys[256];

public:
    Player(int numSegments = 5, float startX = 0.0f, float startY = -15.0f);

    void update();
    void render();
    void processInput(int keys[256]);
    void reset();
    void checkBallCollision(Ball& ball);

    // Getters
    float getX() const { return segments[0].x; }
    float getY() const { return segments[0].y; }
    float getZ() const { return segments[0].z; }

private:
    void updateSegments();
    void applyPhysics();
};

#endif
