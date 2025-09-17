#include "Player.h"

Player::Player(int numSegments, float startX, float startY) {
    segments.resize(numSegments);
    for (int i = 0; i < numSegments; ++i) {
        segments[i].x = startX + (i * 2.0f);
        segments[i].y = startY;
        segments[i].z = 1.0f;
        segments[i].vx = 0.0f;
        segments[i].vy = 0.0f;
        segments[i].vz = 0.0f;
        segments[i].radius = 1.5f - (i * 0.2f);
    }

    // Initialize keys array
    for (int i = 0; i < 256; i++) {
        keys[i] = 0;
    }
}

void Player::update() {
    applyPhysics();
    updateSegments();
}

void Player::render() {
    // Draw head (first segment)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue head
    glPushMatrix();
    glTranslatef(segments[0].x, segments[0].y, segments[0].z);
    glutSolidSphere(segments[0].radius, 20, 20);
    glPopMatrix();

    // Draw body segments
    for (size_t i = 1; i < segments.size(); ++i) {
        glColor3f(0.0f, 0.5f, 1.0f); // Light blue body
        glPushMatrix();
        glTranslatef(segments[i].x, segments[i].y, segments[i].z);
        glutSolidSphere(segments[i].radius, 20, 20);
        glPopMatrix();
    }
}

void Player::processInput(int inputKeys[256]) {
    float force = 0.3f;

    if (inputKeys['w'] || inputKeys['W']) {
        segments[0].vy += force;
    }
    if (inputKeys['s'] || inputKeys['S']) {
        segments[0].vy -= force;
    }
    if (inputKeys['a'] || inputKeys['A']) {
        segments[0].vx -= force;
    }
    if (inputKeys['d'] || inputKeys['D']) {
        segments[0].vx += force;
    }
    if (inputKeys[' ']) {
        // Jump if on ground
        if (segments[0].z <= segments[0].radius + 0.1f) {
            segments[0].vz += 2.0f;
        }
    }

    // Limit maximum speed
    float maxSpeed = 1.5f;
    if (segments[0].vx > maxSpeed) segments[0].vx = maxSpeed;
    if (segments[0].vx < -maxSpeed) segments[0].vx = -maxSpeed;
    if (segments[0].vy > maxSpeed) segments[0].vy = maxSpeed;
    if (segments[0].vy < -maxSpeed) segments[0].vy = -maxSpeed;
}

void Player::reset() {
    for (size_t i = 0; i < segments.size(); i++) {
        segments[i].x = -15.0f + (i * 2.0f);
        segments[i].y = -15.0f;
        segments[i].z = segments[i].radius;
        segments[i].vx = 0.0f;
        segments[i].vy = 0.0f;
        segments[i].vz = 0.0f;
    }
}

void Player::checkBallCollision(Ball& ball) {
    float dx = segments[0].x - ball.getX();
    float dy = segments[0].y - ball.getY();
    float dz = segments[0].z - ball.getZ();
    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    float minDistance = segments[0].radius + ball.getRadius();

    if (distance < minDistance) {
        // Normalize collision vector
        float nx = dx / distance;
        float ny = dy / distance;
        float nz = dz / distance;

        // Push ball away from collision
        ball.setPosition(
            segments[0].x - nx * minDistance,
            segments[0].y - ny * minDistance,
            segments[0].z - nz * minDistance
        );

        // Apply player velocity to ball
        ball.setVelocity(
            segments[0].vx * 1.5f,
            segments[0].vy * 1.5f,
            segments[0].vz * 1.5f
        );
    }
}

void Player::applyPhysics() {
    // Update head position
    segments[0].x += segments[0].vx;
    segments[0].y += segments[0].vy;
    segments[0].z += segments[0].vz;

    // Ground collision for head
    if (segments[0].z < segments[0].radius) {
        segments[0].z = segments[0].radius;
        segments[0].vz = 0;
    }

    // Apply gravity and friction to head
    segments[0].vz -= 0.02f;
    segments[0].vx *= 0.95f;
    segments[0].vy *= 0.95f;
}

void Player::updateSegments() {
    float followSpeed = 0.1f;

    for (size_t i = 1; i < segments.size(); ++i) {
        float dx = segments[i - 1].x - segments[i].x;
        float dy = segments[i - 1].y - segments[i].y;
        float dz = segments[i - 1].z - segments[i].z;

        float distance = sqrt(dx * dx + dy * dy + dz * dz);
        float minDistance = segments[i - 1].radius + segments[i].radius;

        if (distance > minDistance) {
            segments[i].vx = dx * followSpeed;
            segments[i].vy = dy * followSpeed;
            segments[i].vz = dz * followSpeed;
        } else {
            float overlap = minDistance - distance;
            segments[i].x -= dx / distance * overlap;
            segments[i].y -= dy / distance * overlap;
            segments[i].z -= dz / distance * overlap;
            segments[i].vx = 0;
            segments[i].vy = 0;
            segments[i].vz = 0;
        }

        // Ground collision
        if (segments[i].z < segments[i].radius) {
            segments[i].z = segments[i].radius;
            segments[i].vz = 0;
        }

        // Apply gravity and update position
        segments[i].vz -= 0.02f;
        segments[i].x += segments[i].vx;
        segments[i].y += segments[i].vy;
        segments[i].z += segments[i].vz;
    }
}
