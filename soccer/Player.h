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
    float angle; // Individual segment angle for wiggling
};

enum TeamColor {
    TEAM_BRAZIL,
    TEAM_GERMANY
};

enum PlayerState {
    STATE_IDLE,
    STATE_ALIGNING,
    STATE_ATTACKING,
    STATE_COOLDOWN
};

class Player {
private:
    std::vector<PlayerSegment> segments;
    std::vector<float> angles; // Cumulative angles for wiggling effect
    int keys[256];
    TeamColor teamColor;
    float t; // Time parameter for animation
    float rotation; // Player's rotation angle (facing direction)
    
    // AI behavior variables
    PlayerState currentState;
    float stateTimer;
    float detectionRadius;
    float targetRotation;
    float rotationSpeed;
    float attackForce;
    
    // Store initial position for reset
    float initialX, initialY;

public:
    Player(int numSegments = 5, float startX = 0.0f, float startY = -15.0f, TeamColor team = TEAM_BRAZIL);

    void update();
    void render();
    void updateAI(Ball& ball); // New AI update method
    void reset();
    void setInitialPosition(float x, float y); // New method to set initial position
    void checkBallCollision(Ball& ball);

    // Getters
    float getX() const { return segments[0].x; }
    float getY() const { return segments[0].y; }
    float getZ() const { return segments[0].z; }
    float getRotation() const { return rotation; }

private:
    void updateSegments();
    void applyPhysics();
    void setSegmentColor(int segmentIndex);
    float getDistanceToBall(const Ball& ball) const;
    float getAngleToBall(const Ball& ball) const;
    void transitionToState(PlayerState newState);
};

#endif