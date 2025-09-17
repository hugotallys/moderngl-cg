#ifndef CAMERA_H
#define CAMERA_H

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>

class Camera {
private:
    float distance;      // Distance from target
    float angleX;        // Vertical angle (pitch)
    float angleY;        // Horizontal angle (yaw)
    float targetX, targetY, targetZ;  // Look-at target
    float fovY;          // Field of view
    int windowWidth, windowHeight;

    // Mouse state
    int lastMouseX, lastMouseY;
    bool isDragging;

public:
    Camera(float dist = 80.0f, float fov = 75.0f, int winWidth = 800, int winHeight = 600);

    // Camera control methods
    void setTarget(float x, float y, float z);
    void setDistance(float dist);
    void setAngles(float angleX, float angleY);
    void zoom(float delta);
    void rotate(float deltaX, float deltaY);
    void processKeyboardZoom(int keys[256]);

    // Mouse event handlers
    void handleMouseClick(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

    // Apply camera transformation
    void applyView();
    void setupProjection();

    // Getters
    float getDistance() const { return distance; }
    float getAngleX() const { return angleX; }
    float getAngleY() const { return angleY; }
    float getTargetX() const { return targetX; }
    float getTargetY() const { return targetY; }
    float getTargetZ() const { return targetZ; }

private:
    void clampAngles();
    void calculatePosition(float& x, float& y, float& z);
};

#endif
