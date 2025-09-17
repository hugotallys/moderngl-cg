#include "Camera.h"

#include <iostream>

Camera::Camera(float dist, float fov, int winWidth, int winHeight)
    : distance(dist), fovY(fov), windowWidth(winWidth), windowHeight(winHeight),
      angleX(30.0f), angleY(0.0f), targetX(0.0f), targetY(0.0f), targetZ(0.0f),
      lastMouseX(0), lastMouseY(0), isDragging(false) {}

void Camera::setTarget(float x, float y, float z) {
    targetX = x;
    targetY = y;
    targetZ = z;
}

void Camera::setDistance(float dist) {
    distance = dist;
    if (distance < 5.0f) distance = 5.0f;
    if (distance > 200.0f) distance = 200.0f;
}

void Camera::setAngles(float newAngleX, float newAngleY) {
    angleX = newAngleX;
    angleY = newAngleY;
    clampAngles();
}

void Camera::zoom(float delta) {
    distance -= delta * 5.0f;
    setDistance(distance); // This will clamp the values
}

void Camera::rotate(float deltaX, float deltaY) {
    angleY += deltaX * 0.5f;
    angleX += deltaY * 0.5f;
    clampAngles();
}

void Camera::processKeyboardZoom(int keys[256]) {
    if (keys['i'] || keys['I']) {
        std::cout << "Zooming In" << std::endl;
        zoom(1.0f); // Zoom in
    }
    if (keys['o'] || keys['O']) {
        std::cout << "Zooming Out" << std::endl;
        zoom(-1.0f); // Zoom out
    }
}

void Camera::handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = false;
        }
    }
    // Mouse wheel handling removed - now using keyboard zoom
}

void Camera::handleMouseMotion(int x, int y) {
    if (isDragging) {
        float deltaX = (float)(x - lastMouseX);
        float deltaY = (float)(y - lastMouseY);

        rotate(deltaX, deltaY);

        lastMouseX = x;
        lastMouseY = y;
    }
}

void Camera::applyView() {
    float camX, camY, camZ;
    calculatePosition(camX, camY, camZ);

    gluLookAt(camX, camY, camZ,
              targetX, targetY, targetZ,
              0.0f, 0.0f, 1.0f);
}

void Camera::setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovY, (double)windowWidth / (double)windowHeight, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void Camera::clampAngles() {
    // Clamp vertical angle to prevent flipping
    if (angleX > 89.0f) angleX = 89.0f;
    if (angleX < -89.0f) angleX = -89.0f;

    // Normalize horizontal angle
    while (angleY > 360.0f) angleY -= 360.0f;
    while (angleY < 0.0f) angleY += 360.0f;
}

void Camera::calculatePosition(float& x, float& y, float& z) {
    // Convert angles to radians
    float angleXRad = angleX * M_PI / 180.0f;
    float angleYRad = angleY * M_PI / 180.0f;

    // Calculate camera position using spherical coordinates
    x = targetX + distance * sin(angleYRad) * cos(angleXRad);
    y = targetY - distance * cos(angleYRad) * cos(angleXRad);
    z = targetZ + distance * sin(angleXRad);
}
