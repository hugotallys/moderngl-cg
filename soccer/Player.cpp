#include "Player.h"

Player::Player(int numSegments, float startX, float startY, TeamColor team) 
    : teamColor(team), t(0.0f), rotation(0.0f), currentState(STATE_IDLE), 
      stateTimer(0.0f), detectionRadius(16.0f), targetRotation(0.0f),
      rotationSpeed(0.05f), attackForce(0.1f), initialX(startX), initialY(startY),
      fieldWidth(105.0f), fieldHeight(68.0f) {
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
    stateTimer += 0.016f; // Assuming ~60 FPS (16ms per frame)
    applyPhysics();
    checkFieldBoundaries(); // Check field boundaries after physics
    updateSegments();
}

void Player::setFieldBoundaries(float width, float height) {
    fieldWidth = width;
    fieldHeight = height;
}

void Player::checkFieldBoundaries() {
    // Get the head segment (first segment)
    PlayerSegment& head = segments[0];
    
    // Check X boundaries (field width)
    if (head.x < -fieldWidth / 2.0f + head.radius) {
        head.x = -fieldWidth / 2.0f + head.radius;
        head.vx = abs(head.vx) * 0.3f; // Bounce back with reduced velocity
    }
    if (head.x > fieldWidth / 2.0f - head.radius) {
        head.x = fieldWidth / 2.0f - head.radius;
        head.vx = -abs(head.vx) * 0.3f; // Bounce back with reduced velocity
    }
    
    // Check Y boundaries (field height)
    if (head.y < -fieldHeight / 2.0f + head.radius) {
        head.y = -fieldHeight / 2.0f + head.radius;
        head.vy = abs(head.vy) * 0.3f; // Bounce back with reduced velocity
    }
    if (head.y > fieldHeight / 2.0f - head.radius) {
        head.y = fieldHeight / 2.0f - head.radius;
        head.vy = -abs(head.vy) * 0.3f; // Bounce back with reduced velocity
    }
}

// ...existing methods remain the same...

void Player::updateAI(Ball& ball) {
    float distanceToBall = getDistanceToBall(ball);
    
    switch (currentState) {
        case STATE_IDLE: {
            // Just wiggle tail, check if ball is nearby
            if (distanceToBall < detectionRadius) {
                transitionToState(STATE_ALIGNING);
                targetRotation = getAngleToBall(ball);
            }
            break;
        }
            
        case STATE_ALIGNING: {
            // Gradually rotate towards the ball
            float angleDiff = targetRotation - rotation;
            
            // Normalize angle difference to [-PI, PI]
            while (angleDiff > M_PI) angleDiff -= 2 * M_PI;
            while (angleDiff < -M_PI) angleDiff += 2 * M_PI;
            
            if (abs(angleDiff) > 0.1f) {
                // Still need to rotate
                if (angleDiff > 0) {
                    rotation += rotationSpeed;
                } else {
                    rotation -= rotationSpeed;
                }
                // Update target rotation in case ball moved
                targetRotation = getAngleToBall(ball);
            } else {
                // Aligned, now attack
                transitionToState(STATE_ATTACKING);
            }
            
            // If ball moved away during alignment, go back to idle
            if (distanceToBall > detectionRadius * 1.5f) {
                transitionToState(STATE_IDLE);
            }
            break;
        }
            
        case STATE_ATTACKING: {
            // Move forward to hit the ball
            segments[0].vx += attackForce * cos(rotation);
            segments[0].vy += attackForce * sin(rotation);
            
            // Transition to cooldown after a short attack duration
            if (stateTimer > 0.3f) { // Attack for 0.3 seconds
                transitionToState(STATE_COOLDOWN);
            }
            break;
        }
            
        case STATE_COOLDOWN: {
            // Do nothing for a while to prevent immediate re-engagement
            if (stateTimer > 2.0f) { // Cooldown for 2 seconds
                transitionToState(STATE_IDLE);
            }
            break;
        }
    }
}

void Player::transitionToState(PlayerState newState) {
    currentState = newState;
    stateTimer = 0.0f;
}

float Player::getDistanceToBall(const Ball& ball) const {
    float dx = segments[0].x - ball.getX();
    float dy = segments[0].y - ball.getY();
    return sqrt(dx * dx + dy * dy);
}

float Player::getAngleToBall(const Ball& ball) const {
    float dx = ball.getX() - segments[0].x;
    float dy = ball.getY() - segments[0].y;
    return atan2(dy, dx);
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

void Player::reset() {
    t = 0.0f;
    rotation = 0.0f; // Reset rotation to face X direction
    currentState = STATE_IDLE;
    stateTimer = 0.0f;
    targetRotation = 0.0f;
    
    for (size_t i = 0; i < segments.size(); i++) {
        segments[i].x = initialX + (i * 2.0f);
        segments[i].y = initialY;
        segments[i].z = segments[i].radius;
        segments[i].vx = 0.0f;
        segments[i].vy = 0.0f;
        segments[i].vz = 0.0f;
        segments[i].angle = 0.0f;
        angles[i] = 0.0f;
    }
}

void Player::setInitialPosition(float x, float y) {
    initialX = x;
    initialY = y;
    
    // Update current position immediately
    for (size_t i = 0; i < segments.size(); i++) {
        segments[i].x = x + (i * 2.0f);
        segments[i].y = y;
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

        // Apply player velocity to ball with more force during attack state
        float forceMultiplier = (currentState == STATE_ATTACKING) ? 3.0f : 1.5f;
        ball.setVelocity(
            segments[0].vx * forceMultiplier,
            segments[0].vy * forceMultiplier,
            segments[0].vz * forceMultiplier
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