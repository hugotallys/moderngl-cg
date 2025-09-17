//
// Created by Hugo Tallys on 12/09/25.
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <stdlib.h>

#include "SoccerField.h"
#include "Ball.h"
#include "Player.h"
#include "Camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Game objects
Camera* camera;

// Game state
int keys[256];

class SoccerGame {
private:
    SoccerField field;
    Ball ball;
    Player player;
    int scoreTeam1, scoreTeam2;

public:
    SoccerGame() : field(68.0f, 105.0f), ball(0.0f, 0.0f, 1.0f, 1.0f),
                   player(5, -15.0f, -15.0f), scoreTeam1(0), scoreTeam2(0) {}

    void update() {
        player.processInput(keys);
        player.update();
        ball.update(field.getWidth(), field.getHeight());
        player.checkBallCollision(ball);
        checkGoals();
    }

    void render() {
        field.render();
        field.renderGoals();
        player.render();
        ball.render();
        drawScore();
    }

    void reset() {
        scoreTeam1 = 0;
        scoreTeam2 = 0;
        ball.reset();
        player.reset();
        printf("Game reset!\n");
    }

    void checkGoals() {
        if (ball.checkGoal(field.getHeight(), field.getGoalWidth(), field.getGoalHeight())) {
            if (ball.getY() > 0) {
                scoreTeam2++;
                printf("GOAL TEAM 2! Score: Team 1: %d - Team 2: %d\n", scoreTeam1, scoreTeam2);
            } else {
                scoreTeam1++;
                printf("GOAL TEAM 1! Score: Team 1: %d - Team 2: %d\n", scoreTeam1, scoreTeam2);
            }
            ball.reset();
            player.reset();
        }
    }

    void drawScore() {
        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "Team 1: %d - Team 2: %d", scoreTeam1, scoreTeam2);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(10, WINDOW_HEIGHT - 20);
        for (int i = 0; scoreText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
};

SoccerGame* game;

void keyboard(unsigned char key, int x, int y) {
    keys[key] = 1;

    switch (key) {
    case 27: // ESC
        exit(0);
        break;
    case 'r':
    case 'R':
        game->reset();
        break;
    }

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = 0;
}

void update(int value) {
    camera->processKeyboardZoom(keys);  // Add this line to process I/O zoom keys
    game->update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

void mouse(int button, int state, int x, int y) {
    camera->handleMouseClick(button, state, x, y);
    glutPostRedisplay();
}

void motion(int x, int y) {
    camera->handleMouseMotion(x, y);
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera->applyView();
    game->render();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Soccer Simulation");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);

    // Initialize camera
    camera = new Camera(80.0f, 75.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    camera->setTarget(0.0f, 0.0f, 0.0f);
    camera->setAngles(30.0f, 0.0f);
    camera->setupProjection();

    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);

    // Initialize game
    game = new SoccerGame();

    // Initialize keys array
    for (int i = 0; i < 256; i++) {
        keys[i] = 0;
    }

    printf("=== 3D SOCCER SIMULATION ===\n");
    printf("Controls:\n");
    printf("WASD - Move player\n");
    printf("SPACE - Jump\n");
    printf("Mouse drag - Control camera\n");
    printf("Mouse wheel - Zoom\n");
    printf("I - Zoom in\n");
    printf("O - Zoom out\n");
    printf("ESC - Exit\n");
    printf("=============================\n");

    glutMainLoop();

    delete game;
    delete camera;
    return 0;
}
