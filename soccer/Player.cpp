#include "Player.h"

Player::Player(int numSegments, float startX, float startY, TeamColor team) 
    : teamColor(team), t(0.0f), rotation(0.0f) {
    segments.resize(numSegments);
    angles.resize(numSegments);
    
    for (int i = 0; i < numSegments; ++i) {
        segments[i].x = startX + (i * 2.0f);
        segments[i].y = startY;
        segments[i].z = 1.0f;
        segments[i].vx = 0.0f;
        segments[i].vy = 0.0f;
        segments[i].vz = 0.0f;
        segments[i].radius = 1.5f - (i * 0.2f);
        segments[i].angle = 0.0f;
        angles[i] = 0.0f;
    }

    // Initialize keys array
    for (int i = 0; i < 256; i++) {
        keys[i] = 0;
    }
}

void Player::update() {
    t += 0.25f; // Increment time parameter
    applyPhysics();
    updateSegments();
}

void Player::setSegmentColor(int segmentIndex) {
    if (teamColor == TEAM_BRAZIL) {
        // Brazil colors: Blue -> Yellow -> Dark Green
        switch (segmentIndex % 3) {
            case 0: glColor3f(0.0f, 0.0f, 1.0f); break; // Blue
            case 1: glColor3f(1.0f, 1.0f, 0.0f); break; // Yellow
            case 2: glColor3f(0.0f, 0.5f, 0.0f); break; // Dark Green
        }
    } else { // TEAM_GERMANY
        // Germany colors: Black -> Yellow -> Red
        switch (segmentIndex % 3) {
            case 0: glColor3f(0.0f, 0.0f, 0.0f); break; // Black
            case 1: glColor3f(1.0f, 1.0f, 0.0f); break; // Yellow
            case 2: glColor3f(1.0f, 0.0f, 0.0f); break; // Red
        }
    }
}

void Player::render() {
    // Draw head (first segment) with team color
    setSegmentColor(0);
    glPushMatrix();
    glTranslatef(segments[0].x, segments[0].y, segments[0].z);
    glutSolidSphere(segments[0].radius, 20, 20);
    glPopMatrix();

    // Draw body segments with wiggling animation
    for (size_t i = 1; i < segments.size(); ++i) {
        setSegmentColor(i);
        glPushMatrix();
        glTranslatef(segments[i].x, segments[i].y, segments[i].z);
        glutSolidSphere(segments[i].radius, 20, 20);
        glPopMatrix();
    }
}

void Player::processInput(int inputKeys[256]) {
    float moveForce = 0.3f;
    float rotationSpeed = 0.1f; // Rotation speed in radians

    // A/D keys rotate the player
    if (inputKeys['a'] || inputKeys['A']) {
        rotation += rotationSpeed; // Rotate counter-clockwise
    }
    if (inputKeys['d'] || inputKeys['D']) {
        rotation -= rotationSpeed; // Rotate clockwise
    }

    // W/S keys move forward/backward in the direction the player is facing
    if (inputKeys['w'] || inputKeys['W']) {
        // Move forward in the direction the player is facing
        segments[0].vx += moveForce * cos(rotation);
        segments[0].vy += moveForce * sin(rotation);
    }
    if (inputKeys['s'] || inputKeys['S']) {
        // Move backward (opposite to facing direction)
        segments[0].vx -= moveForce * cos(rotation);
        segments[0].vy -= moveForce * sin(rotation);
    }

    // Limit maximum speed
    float maxSpeed = 1.5f;
    if (segments[0].vx > maxSpeed) segments[0].vx = maxSpeed;
    if (segments[0].vx < -maxSpeed) segments[0].vx = -maxSpeed;
    if (segments[0].vy > maxSpeed) segments[0].vy = maxSpeed;
    if (segments[0].vy < -maxSpeed) segments[0].vy = -maxSpeed;
}

void Player::reset() {
    t = 0.0f;
    rotation = 0.0f; // Reset rotation to face X direction
    for (size_t i = 0; i < segments.size(); i++) {
        segments[i].x = -15.0f + (i * 2.0f);
        segments[i].y = -15.0f;
        segments[i].z = segments[i].radius;
        segments[i].vx = 0.0f;
        segments[i].vy = 0.0f;
        segments[i].vz = 0.0f;
        segments[i].angle = 0.0f;
        angles[i] = 0.0f;
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
    // Update head position (only the first segment is subject to physics)
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
    float dist = 0.0f;
    float radius = segments[0].radius;
    float radDec = 0.2f; // Radius decrease per segment
    
    // Calculate angles based on the JavaScript wiggling logic
    for (size_t i = 0; i < segments.size(); ++i) {
        if (i > 0) {
            dist += radius + (radius - radDec);
            
            // Calculate individual angle for this segment (similar to JS code)
            angles[i] = 0.15f * sin(t - i * 0.9f);
            
            // Add cumulative angle from previous segment
            if (i > 1) {
                angles[i] += angles[i - 1];
            }
            
            // Add player's rotation to the wiggling angle for proper orientation
            // The tail should face backwards, so we add PI to reverse the direction
            float totalAngle = angles[i] + rotation + M_PI;
            
            // Position segment based on wiggling angles and player rotation
            // Use positive values to make segments trail behind (opposite direction)
            segments[i].x = segments[0].x + dist * cos(totalAngle);
            segments[i].y = segments[0].y + dist * sin(totalAngle);
            
            // Keep segments slightly above ground with some vertical oscillation
            segments[i].z = segments[i].radius + abs(sin(t + i * 0.5f)) * 0.3f;
            
            // Update radius for next segment
            radius -= radDec;
            if (radius < 0.5f) radius = 0.5f; // Minimum radius
            segments[i].radius = radius;
        }
    }
}