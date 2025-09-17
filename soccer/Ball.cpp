#include "Ball.h"

Ball::Ball(float startX, float startY, float startZ, float r)
    : x(startX), y(startY), z(startZ), vx(0.0f), vy(0.0f), vz(0.0f), radius(r) {}

void Ball::update(float fieldWidth, float fieldHeight) {
    // Update position
    x += vx;
    y += vy;
    z += vz;

    // Apply friction
    vx *= 0.98f;
    vy *= 0.98f;
    vz *= 0.95f;

    // Field boundary collisions
    if (x < -fieldWidth / 2 + radius) {
        x = -fieldWidth / 2 + radius;
        vx = -vx * 0.7f;
    }
    if (x > fieldWidth / 2 - radius) {
        x = fieldWidth / 2 - radius;
        vx = -vx * 0.7f;
    }
    if (y > fieldHeight / 2 - radius) {
        y = fieldHeight / 2 - radius;
        vy = -vy * 0.7f;
    }
    if (y < -fieldHeight / 2 + radius) {
        y = -fieldHeight / 2 + radius;
        vy = -vy * 0.7f;
    }

    // Ground collision
    if (z < radius) {
        z = radius;
        vz = -vz * 0.6f; // Bounce
        if (fabs(vz) < 0.1f) vz = 0; // Stop small bounces
    }

    // Apply gravity
    vz -= 0.02f;
}

void Ball::render() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f); // White ball
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

void Ball::reset() {
    x = 0.0f;
    y = 0.0f;
    z = radius;
    vx = 0.0f;
    vy = 0.0f;
    vz = 0.0f;
}

void Ball::applyForce(float fx, float fy, float fz) {
    vx += fx;
    vy += fy;
    vz += fz;
}

void Ball::setPosition(float newX, float newY, float newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

void Ball::setVelocity(float newVx, float newVy, float newVz) {
    vx = newVx;
    vy = newVy;
    vz = newVz;
}

bool Ball::checkGoal(float fieldHeight, float goalWidth, float goalHeight) {
    // Goal in top area
    if (y > fieldHeight / 2 && x > -goalWidth / 2 && x < goalWidth / 2 && z < goalHeight) {
        return true;
    }
    // Goal in bottom area
    if (y < -fieldHeight / 2 && x > -goalWidth / 2 && x < goalWidth / 2 && z < goalHeight) {
        return true;
    }
    return false;
}
