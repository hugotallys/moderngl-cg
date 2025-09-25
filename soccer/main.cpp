#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <iostream>

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
int specialKeys[256];

class SoccerGame {
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
        
        // Handle arrow keys for ball movement
        handleBallMovement();
        
        ball.update(field.getWidth(), field.getHeight(), field.getGoalWidth(), field.getGoalHeight());

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
        std::cout << "Game reset!" << std::endl;
    }

    void checkGoals() {
        if (ball.checkGoal(field.getHeight(), field.getGoalWidth(), field.getGoalHeight())) {
            if (ball.getY() > 0) {
                scoreTeam2++;
                std::cout << "GOAL TEAM 2! Score: Team 1: " << scoreTeam1 << " - Team 2: " << scoreTeam2 << std::endl;
            } else {
                scoreTeam1++;
                std::cout << "GOAL TEAM 1! Score: Team 1: " << scoreTeam1 << " - Team 2: " << scoreTeam2 << std::endl;
            }
            ball.reset();
            player.reset();
        }
    }

    void handleBallMovement() {
        const float BALL_FORCE = 0.3f; // Constant force applied by arrow keys
        
        if (specialKeys[GLUT_KEY_UP]) {
            ball.applyForce(0.0f, BALL_FORCE, 0.0f);
        }
        if (specialKeys[GLUT_KEY_DOWN]) {
            ball.applyForce(0.0f, -BALL_FORCE, 0.0f);
        }
        if (specialKeys[GLUT_KEY_LEFT]) {
            ball.applyForce(-BALL_FORCE, 0.0f, 0.0f);
        }
        if (specialKeys[GLUT_KEY_RIGHT]) {
            ball.applyForce(BALL_FORCE, 0.0f, 0.0f);
        }
    }

    void drawScore() const {
        glPushMatrix(); // Save the current modelview matrix

        // Scoreboard dimensions (doubled in size)
        const float boardWidth = 65.0f;
        const float boardHeight = 20.0f;
        const float headerHeight = 10.0f; // The top part of the board for team names

        // Position the scoreboard at the midfield line, on the side, and raised up.
        // The center of the board will be raised up by 5 units and placed off the side of the field.
        glTranslatef(- (field.getWidth() / 2.0f + 5.0f), 0.0f, boardHeight / 2.0f + 5.0f);

        // Rotate the board to be vertical and face the center of the field (+X direction)
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

        // --- Draw Scoreboard ---

        // 1. Draw the black background rectangle
        glColor3f(0.0f, 0.0f, 0.0f); // Black color
        glBegin(GL_QUADS);
            glVertex2f(-boardWidth / 2.0f, -boardHeight / 2.0f);
            glVertex2f( boardWidth / 2.0f, -boardHeight / 2.0f);
            glVertex2f( boardWidth / 2.0f,  boardHeight / 2.0f);
            glVertex2f(-boardWidth / 2.0f,  boardHeight / 2.0f);
        glEnd();

        // Small Z-offset for lines and text to prevent z-fighting
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.01f);

        // 2. Draw the white outline and dividing lines
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glLineWidth(4.0f);

        // Outline
        glBegin(GL_LINE_LOOP);
            glVertex2f(-boardWidth / 2.0f, -boardHeight / 2.0f);
            glVertex2f( boardWidth / 2.0f, -boardHeight / 2.0f);
            glVertex2f( boardWidth / 2.0f,  boardHeight / 2.0f);
            glVertex2f(-boardWidth / 2.0f,  boardHeight / 2.0f);
        glEnd();

        // Vertical divider
        glBegin(GL_LINES);
            glVertex2f(0.0f, -boardHeight / 2.0f);
            glVertex2f(0.0f,  boardHeight / 2.0f);
        glEnd();

        // Horizontal divider for header
        glBegin(GL_LINES);
            glVertex2f(-boardWidth / 2.0f, boardHeight / 2.0f - headerHeight);
            glVertex2f( boardWidth / 2.0f, boardHeight / 2.0f - headerHeight);
        glEnd();
        glLineWidth(1.0f); // Reset line width

        // --- Draw Text ---

        // Helper function to draw stroke text, centered in a given box.
        auto drawCenteredText = [](float box_cx, float box_cy, const char* text, float scale) {
            glPushMatrix();
            float textWidth = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)text);

            // Approx height of GLUT_STROKE_ROMAN is ~119 units.
            // We translate to center the text block.
            glTranslatef(box_cx - (textWidth / 2.0f) * scale, box_cy - (119.05f / 3.0f) * scale, 0.0f);
            glScalef(scale, scale, scale);

            for (const char* c = text; *c != '\0'; c++) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            glPopMatrix();
        };

        // Calculate center Y positions for the cells
        float header_cy = (boardHeight / 2.0f + (boardHeight / 2.0f - headerHeight)) / 2.0f; // Midpoint of top cell
        float score_cy = ((boardHeight / 2.0f - headerHeight) + (-boardHeight / 2.0f)) / 2.0f; // Midpoint of bottom cell

        glLineWidth(4.0f);

        // Team names
        drawCenteredText(-boardWidth / 4.0f, header_cy, "Team Blue", 0.04f);
        drawCenteredText( boardWidth / 4.0f, header_cy, "Team Red", 0.04f);

        // Scores
        char score1Str[4];
        snprintf(score1Str, sizeof(score1Str), "%d", scoreTeam1);
        char score2Str[4];
        snprintf(score2Str, sizeof(score2Str), "%d", scoreTeam2);

        drawCenteredText(-boardWidth / 4.0f, score_cy, score1Str, 0.05f);
        drawCenteredText( boardWidth / 4.0f, score_cy, score2Str, 0.05f);

        glPopMatrix(); // Pop the Z-offset matrix
        glPopMatrix(); // Restore the original modelview matrix
    }

    void ballJump() {
        constexpr float JUMP_FORCE = 4.0f; // Adjust the force as needed
        ball.applyForce(0.0f, 0.0f, JUMP_FORCE);
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
    case 'j':
    case 'J':
        game->ballJump();
        break;
    }

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = 0;
}

void specialKeyboard(int key, int x, int y) {
    specialKeys[key] = 1;
    glutPostRedisplay();
}

void specialKeyboardUp(int key, int x, int y) {
    specialKeys[key] = 0;
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
    camera->setAngles(90.0f, 0.0f); // change to angle Z
    camera->setupProjection();

    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(16, update, 0);

    // Initialize game
    game = new SoccerGame();

    // Initialize keys array
    for (int i = 0; i < 256; i++) {
        keys[i] = 0;
        specialKeys[i] = 0;
    }

    printf("=== 3D SOCCER SIMULATION ===\n");
    printf("Controls:\n");
    printf("WASD - Move player\n");
    printf("Arrow keys - Move ball\n");
    printf("SPACE - Jump\n");
    printf("Mouse drag - Control camera\n");
    printf("Mouse wheel - Zoom\n");
    printf("I - Zoom in\n");
    printf("O - Zoom out\n");
    printf("R - Reset game\n");
    printf("ESC - Exit\n");
    printf("=============================\n");

    glutMainLoop();

    delete game;
    delete camera;
    return 0;
}
