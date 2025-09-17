//
// Created by Hugo Tallys on 12/09/25.
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Variaveis para controlar a camera
float cam_x, cam_y, cam_z;
float center_x, center_y, center_z;
float fov_y;

// Variaveis de controle do mouse para zoom e orbita
float cam_dist;
float cam_angle_x, cam_angle_y;
int mouse_x, mouse_y;
int mouse_button;

// Variaveis para as dimensoes do campo de futebol
float field_width;
float field_height;

// Estrutura para representar um segmento do jogador (bola)
struct PlayerSegment
{
    float x, y, z;
    float vx, vy, vz;
    float radius;
};

// Variaveis do jogador (snake)
std::vector<PlayerSegment> player_snake;
int num_segments = 5;

// Variaveis da bola do jogo (a ser chutada)
float ball_x, ball_y, ball_z;
float ball_vx, ball_vy, ball_vz;
float ball_radius;
float ball_speed;

// Variaveis do jogo
int score_team1, score_team2;
float goal_width, goal_height, goal_depth;

// Teclas pressionadas
int keys[256];

/*
 * Funcao para desenhar uma esfera (bola)
 */
void drawBall(float x, float y, float z, float radius)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para a bola
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

/*
 * Funcao para desenhar o jogador (snake)
 */
void drawPlayer()
{
    // Desenha a cabeca do jogador (primeiro segmento)
    glColor3f(0.0f, 0.0f, 1.0f); // Azul para a cabeca
    glPushMatrix();
    glTranslatef(player_snake[0].x, player_snake[0].y, player_snake[0].z);
    glutSolidSphere(player_snake[0].radius, 20, 20);
    glPopMatrix();

    // Desenha os segmentos seguintes
    for (size_t i = 1; i < player_snake.size(); ++i)
    {
        glColor3f(0.0f, 0.5f, 1.0f); // Azul mais claro para o corpo
        glPushMatrix();
        glTranslatef(player_snake[i].x, player_snake[i].y, player_snake[i].z);
        glutSolidSphere(player_snake[i].radius, 20, 20);
        glPopMatrix();
    }
}

/*
 * Funcao para desenhar um gol
 */
void drawGoal(float x, float y, float z, float width, float height, float depth)
{
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para os postes

    // Poste esquerdo
    glPushMatrix();
    glTranslatef(x - width / 2, y, z + height / 2);
    glScalef(0.5f, 0.5f, height);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Poste direito
    glPushMatrix();
    glTranslatef(x + width / 2, y, z + height / 2);
    glScalef(0.5f, 0.5f, height);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Trave superior
    glPushMatrix();
    glTranslatef(x, y, z + height);
    glScalef(width, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rede (representada por linhas)
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (int i = 0; i <= 10; i++)
    {
        float pos = x - width / 2 + (width * i / 10.0f);
        glVertex3f(pos, y - depth, z);
        glVertex3f(pos, y - depth, z + height);
    }
    for (int i = 0; i <= 10; i++)
    {
        float pos = z + (height * i / 10.0f);
        glVertex3f(x - width / 2, y - depth, pos);
        glVertex3f(x + width / 2, y - depth, pos);
    }
    glEnd();
}

/*
 * Funcao para desenhar o campo de futebol com marcacoes
 */
void drawSoccerField(float width, float height)
{
    // Desenha o campo base
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_TRIANGLES);
    // Primeiro triangulo (bottom-left, bottom-right, top-right)
    glVertex3f(-width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, height / 2.0f, 0.0f);

    // Segundo triangulo (bottom-left, top-right, top-left)
    glVertex3f(-width / 2.0f, -height / 2.0f, 0.0f);
    glVertex3f(width / 2.0f, height / 2.0f, 0.0f);
    glVertex3f(-width / 2.0f, height / 2.0f, 0.0f);
    glEnd();

    // Desenha as linhas do campo
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // Bordas do campo
    glVertex3f(-width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, height / 2, 0.01f);
    glVertex3f(width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, -height / 2, 0.01f);

    // Linha do meio
    glVertex3f(-width / 2, 0, 0.01f);
    glVertex3f(width / 2, 0, 0.01f);

    // Circulo central
    for (int i = 0; i < 360; i += 10)
    {
        float angle1 = i * M_PI / 180.0f;
        float angle2 = (i + 10) * M_PI / 180.0f;
        glVertex3f(cos(angle1) * 9.15f, sin(angle1) * 9.15f, 0.01f);
        glVertex3f(cos(angle2) * 9.15f, sin(angle2) * 9.15f, 0.01f);
    }

    // Areas dos gols
    float area_width = 16.5f;
    float area_height = 40.32f;

    // Area do gol 1
    glVertex3f(-area_width / 2, height / 2, 0.01f);
    glVertex3f(area_width / 2, height / 2, 0.01f);
    glVertex3f(area_width / 2, height / 2, 0.01f);
    glVertex3f(area_width / 2, height / 2 - area_height, 0.01f);
    glVertex3f(area_width / 2, height / 2 - area_height, 0.01f);
    glVertex3f(-area_width / 2, height / 2 - area_height, 0.01f);
    glVertex3f(-area_width / 2, height / 2 - area_height, 0.01f);
    glVertex3f(-area_width / 2, height / 2, 0.01f);

    // Area do gol 2
    glVertex3f(-area_width / 2, -height / 2, 0.01f);
    glVertex3f(area_width / 2, -height / 2, 0.01f);
    glVertex3f(area_width / 2, -height / 2, 0.01f);
    glVertex3f(area_width / 2, -height / 2 + area_height, 0.01f);
    glVertex3f(area_width / 2, -height / 2 + area_height, 0.01f);
    glVertex3f(-area_width / 2, -height / 2 + area_height, 0.01f);
    glVertex3f(-area_width / 2, -height / 2 + area_height, 0.01f);
    glVertex3f(-area_width / 2, -height / 2, 0.01f);

    glEnd();
}

/*
 * Funcao para verificar gols
 */
void checkGoal()
{
    // Gol no time 1 (parte superior do campo)
    if (ball_y > field_height / 2 && ball_x > -goal_width / 2 && ball_x < goal_width / 2 && ball_z < goal_height)
    {
        score_team2++;
        printf("GOL DO TIME 2! Placar: Time 1: %d - Time 2: %d\n", score_team1, score_team2);
        // Reinicia a bola no centro
        ball_x = 0;
        ball_y = 0;
        ball_z = ball_radius;
        ball_vx = 0;
        ball_vy = 0;
        ball_vz = 0;
        player_snake[0].x = 0;
        player_snake[0].y = 0;
        player_snake[0].z = 0;
    }

    // Gol no time 2 (parte inferior do campo)
    if (ball_y < -field_height / 2 && ball_x > -goal_width / 2 && ball_x < goal_width / 2 && ball_z < goal_height)
    {
        score_team1++;
        printf("GOL DO TIME 1! Placar: Time 1: %d - Time 2: %d\n", score_team1, score_team2);
        // Reinicia a bola no centro
        ball_x = 0;
        ball_y = 0;
        ball_z = ball_radius;
        ball_vx = 0;
        ball_vy = 0;
        ball_vz = 0;
        player_snake[0].x = 0;
        player_snake[0].y = 0;
        player_snake[0].z = 0;
    }
}

/*
 * Funcao para verificar colisao entre o jogador e a bola
 */
void checkPlayerBallCollision()
{
    float dx = player_snake[0].x - ball_x;
    float dy = player_snake[0].y - ball_y;
    float dz = player_snake[0].z - ball_z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    float min_distance = player_snake[0].radius + ball_radius;

    if (distance < min_distance)
    {
        // Normaliza o vetor de direcao da colisao
        float nx = dx / distance;
        float ny = dy / distance;
        float nz = dz / distance;

        // "Empurra" a bola para fora da colisao para evitar grudar
        ball_x = player_snake[0].x - nx * min_distance;
        ball_y = player_snake[0].y - ny * min_distance;
        ball_z = player_snake[0].z - nz * min_distance;

        // Calcula a nova velocidade da bola baseada na velocidade do jogador
        // Este e o efeito de "bump" do bilhar
        ball_vx = player_snake[0].vx * 1.5f; // Aumenta um pouco a forca
        ball_vy = player_snake[0].vy * 1.5f;
        ball_vz = player_snake[0].vz * 1.5f;
    }
}

/*
 * Funcao para atualizar a fisica da bola
 */
void updateBall()
{
    // Atualiza posicao
    ball_x += ball_vx;
    ball_y += ball_vy;
    ball_z += ball_vz;

    // Aplicar fricção
    ball_vx *= 0.98f;
    ball_vy *= 0.98f;
    ball_vz *= 0.95f; // Mais fricção no eixo Z

    // Colisao com as bordas do campo
    if (ball_x < -field_width / 2 + ball_radius)
    {
        ball_x = -field_width / 2 + ball_radius;
        ball_vx = -ball_vx * 0.7f;
    }
    if (ball_x > field_width / 2 - ball_radius)
    {
        ball_x = field_width / 2 - ball_radius;
        ball_vx = -ball_vx * 0.7f;
    }
    if (ball_y > field_height / 2 - ball_radius)
    {
        ball_y = field_height / 2 - ball_radius;
        ball_vy = -ball_vy * 0.7f;
    }
    if (ball_y < -field_height / 2 + ball_radius)
    {
        ball_y = -field_height / 2 + ball_radius;
        ball_vy = -ball_vy * 0.7f;
    }

    // Colisao com o chao
    if (ball_z < ball_radius)
    {
        ball_z = ball_radius;
        ball_vz = -ball_vz * 0.6f; // Quique
        if (fabs(ball_vz) < 0.1f) ball_vz = 0; // Para pequenos quiques
    }

    // Gravidade
    ball_vz -= 0.02f;

    checkGoal();
}

/*
 * Funcao para atualizar a fisica do jogador
 */
void updatePlayer()
{
    // Atualiza a posicao da cabeca
    player_snake[0].x += player_snake[0].vx;
    player_snake[0].y += player_snake[0].vy;
    player_snake[0].z += player_snake[0].vz;

    // Colisao da cabeca do jogador com o chao
    if (player_snake[0].z < player_snake[0].radius)
    {
        player_snake[0].z = player_snake[0].radius;
        player_snake[0].vz = 0;
    }
    // Gravidade para a cabeca
    player_snake[0].vz -= 0.02f;

    // Aplica fricção a cabeca do jogador
    player_snake[0].vx *= 0.95f;
    player_snake[0].vy *= 0.95f;

    // Move os segmentos do corpo para seguir a cabeca
    float follow_speed = 0.1f;
    for (size_t i = 1; i < player_snake.size(); ++i)
    {
        float prev_x = player_snake[i - 1].x;
        float prev_y = player_snake[i - 1].y;
        float prev_z = player_snake[i - 1].z;
        float curr_x = player_snake[i].x;
        float curr_y = player_snake[i].y;
        float curr_z = player_snake[i].z;

        float dx = prev_x - curr_x;
        float dy = prev_y - curr_y;
        float dz = prev_z - curr_z;

        float distance = sqrt(dx * dx + dy * dy + dz * dz);
        float min_distance = player_snake[i - 1].radius + player_snake[i].radius;

        // Se o segmento estiver muito longe, move-o para perto
        if (distance > min_distance)
        {
            player_snake[i].vx = dx * follow_speed;
            player_snake[i].vy = dy * follow_speed;
            player_snake[i].vz = dz * follow_speed;
        }
        else
        {
            // Se estiver perto, ajusta a posicao para evitar sobreposicao
            float overlap = min_distance - distance;
            player_snake[i].x -= dx / distance * overlap;
            player_snake[i].y -= dy / distance * overlap;
            player_snake[i].z -= dz / distance * overlap;
            player_snake[i].vx = 0;
            player_snake[i].vy = 0;
            player_snake[i].vz = 0;
        }

        // Colisao com o chao
        if (player_snake[i].z < player_snake[i].radius)
        {
            player_snake[i].z = player_snake[i].radius;
            player_snake[i].vz = 0;
        }

        // Gravidade para os segmentos do corpo
        player_snake[i].vz -= 0.02f;

        // Aplica a velocidade calculada
        player_snake[i].x += player_snake[i].vx;
        player_snake[i].y += player_snake[i].vy;
        player_snake[i].z += player_snake[i].vz;
    }
}

/*
 * Funcao para desenhar o placar na tela
 */
void drawScore()
{
    char scoreText[50];
    sprintf(scoreText, "Time 1: %d - Time 2: %d", score_team1, score_team2);

    // Salva as matrizes atuais
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desenha o texto
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10, WINDOW_HEIGHT - 20);
    for (int i = 0; scoreText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    // Restaura as matrizes
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Funcao para lidar com teclas pressionadas
 */
void keyboard(unsigned char key, int x, int y)
{
    keys[key] = 1;

    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;
    case 'r':
    case 'R':
        // Reinicia o jogo
        score_team1 = 0;
        score_team2 = 0;
        ball_x = 0;
        ball_y = 0;
        ball_z = ball_radius;
        ball_vx = 0;
        ball_vy = 0;
        ball_vz = 0;
        player_snake[0].x = 0;
        player_snake[0].y = 0;
        player_snake[0].z = player_snake[0].radius;
        player_snake[0].vx = 0;
        player_snake[0].vy = 0;
        player_snake[0].vz = 0;
        for (size_t i = 1; i < player_snake.size(); i++)
        {
            player_snake[i].x = 0;
            player_snake[i].y = 0;
            player_snake[i].z = player_snake[i].radius;
            player_snake[i].vx = 0;
            player_snake[i].vy = 0;
            player_snake[i].vz = 0;
        }
        printf("Jogo reiniciado!\n");
        break;
    }

    glutPostRedisplay();
}

/*
 * Funcao para lidar com teclas liberadas
 */
void keyboardUp(unsigned char key, int x, int y)
{
    keys[key] = 0;
}

/*
 * Funcao para processar entrada continua do teclado
 */
void processKeys()
{
    float force = 0.3f;

    if (keys['w'] || keys['W'])
    {
        player_snake[0].vy += force;
    }
    if (keys['s'] || keys['S'])
    {
        player_snake[0].vy -= force;
    }
    if (keys['a'] || keys['A'])
    {
        player_snake[0].vx -= force;
    }
    if (keys['d'] || keys['D'])
    {
        player_snake[0].vx += force;
    }
    if (keys[' '])
    {
        // Espaco para saltar
        if (player_snake[0].z <= player_snake[0].radius + 0.1f)
        {
            player_snake[0].vz += 2.0f;
        }
    }

    // Limita a velocidade maxima do jogador
    float max_speed = 1.5f;
    if (player_snake[0].vx > max_speed) player_snake[0].vx = max_speed;
    if (player_snake[0].vx < -max_speed) player_snake[0].vx = -max_speed;
    if (player_snake[0].vy > max_speed) player_snake[0].vy = max_speed;
    if (player_snake[0].vy < -max_speed) player_snake[0].vy = -max_speed;
}

/*
 * Funcao de atualizacao (timer)
 */
void update(int value)
{
    processKeys();
    updatePlayer();
    updateBall();
    checkPlayerBallCollision();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

/*
 * Funcao para lidar com eventos do mouse
 */
void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_x = x;
    mouse_y = y;

    if (button == 3) // Scroll up
    {
        cam_dist -= 5.0f; // Zoom in
        if (cam_dist < 5.0f) cam_dist = 5.0f; // Limite minimo
    }
    else if (button == 4) // Scroll down
    {
        cam_dist += 5.0f; // Zoom out
    }

    // Solicita que a tela seja redesenhada
    glutPostRedisplay();
}

/*
 * Funcao para lidar com o movimento do mouse enquanto um botao esta pressionado
 */
void motion(int x, int y)
{
    float dx = (float)(x - mouse_x);
    float dy = (float)(y - mouse_y);

    if (mouse_button == GLUT_LEFT_BUTTON)
    {
        // Orbita a camera ao redor do centro
        cam_angle_y += dx * 0.5f;
        cam_angle_x += dy * 0.5f;

        // Limita a rotacao vertical para evitar "giros" indesejados
        if (cam_angle_x > 89.0f) cam_angle_x = 89.0f;
        if (cam_angle_x < -89.0f) cam_angle_x = -89.0f;
    }

    mouse_x = x;
    mouse_y = y;

    // Solicita que a tela seja redesenhada
    glutPostRedisplay();
}

/*
 * Funcao de renderizacao principal
 */
void display()
{
    // Limpa os buffers de cor e profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Seleciona a matriz ModelView e reseta as transformacoes
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calcula a posicao da camera com base na distancia e nos angulos de orbita
    cam_x = center_x + cam_dist * sin(cam_angle_y * M_PI / 180.0f) * cos(cam_angle_x * M_PI / 180.0f);
    cam_y = center_y + cam_dist * sin(cam_angle_x * M_PI / 180.0f);
    cam_z = center_z + cam_dist * cos(cam_angle_y * M_PI / 180.0f) * cos(cam_angle_x * M_PI / 180.0f);

    // Define a matriz de visualizacao (camera)
    gluLookAt(cam_x, cam_y, cam_z,
              center_x, center_y, center_z,
              0.0f, 0.0f, 1.0f); // Up vector aponta para o eixo Z

    // Desenha o campo de futebol
    drawSoccerField(field_width, field_height);

    // Desenha os gols
    drawGoal(0, field_height / 2 + goal_depth / 2, 0, goal_width, goal_height, goal_depth);
    drawGoal(0, -field_height / 2 - goal_depth / 2, 0, goal_width, goal_height, goal_depth);

    // Desenha o jogador
    drawPlayer();

    // Desenha a bola do jogo
    drawBall(ball_x, ball_y, ball_z, ball_radius);

    // Desenha o placar
    drawScore();

    // Troca os buffers para exibir o que foi renderizado
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // Inicializacao da GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Simulacao 3D de Futebol");

    // Habilita teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // Registra as funcoes de callback
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);

    // Inicializa a matriz de projecao (perspectiva)
    fov_y = 75.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_y, 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.001f, 1000.0f);

    // Inicializa a camera para uma visao inicial
    cam_dist = 80.0f;
    cam_angle_x = 30.0f;
    cam_angle_y = 0.0f;

    // O ponto que a camera esta olhando (centro do campo)
    center_x = 0.0f;
    center_y = 0.0f;
    center_z = 0.0f;

    // Define as dimensoes do campo de futebol (padrao FIFA)
    field_width = 68.0f;
    field_height = 105.0f;

    // Inicializa a bola
    ball_x = 0.0f;
    ball_y = 0.0f;
    ball_z = 1.0f;
    ball_vx = 0.0f;
    ball_vy = 0.0f;
    ball_vz = 0.0f;
    ball_radius = 1.0f;

    // Inicializa o jogador (snake)
    player_snake.resize(num_segments);
    for (int i = 0; i < num_segments; ++i)
    {
        player_snake[i].x = -15.0f + (i * 2.0f);
        player_snake[i].y = -15.0f;
        player_snake[i].z = 1.0f;
        player_snake[i].vx = 0;
        player_snake[i].vy = 0;
        player_snake[i].vz = 0;
        player_snake[i].radius = 1.5f - (i * 0.2f);
    }

    // Inicializa os gols
    goal_width = 7.32f;
    goal_height = 2.44f;
    goal_depth = 2.0f;

    // Inicializa o placar
    score_team1 = 0;
    score_team2 = 0;

    // Inicializa o array de teclas
    for (int i = 0; i < 256; i++)
    {
        keys[i] = 0;
    }

    printf("=== SIMULACAO 3D DE FUTEBOL ===\n");
    printf("Controles:\n");
    printf("WASD - Mover o jogador (cabeca da snake)\n");
    printf("ESPACO - Fazer o jogador saltar\n");
    printf("Mouse - Controlar camera\n");
    printf("Scroll - Zoom\n");
    printf("R - Reiniciar jogo\n");
    printf("ESC - Sair\n");
    printf("===============================\n");

    // Inicia o loop principal da GLUT
    glutMainLoop();

    return 0;
}
