#include <GL/glut.h>
#include <math.h>
#include <cstdio> 
#include "scene.h"
#include "utils/maploader.h"

extern MapLoader gMap;

#define BLOCO_TAMANHO 4.0f  
// Raio grande para não "cortar" a parede com a câmera
#define PLAYER_RAIO   1.0f   
#define VELOCIDADE    0.15f

bool verificaParede(float x, float z) {
    // FÓRMULA CORRETA PARA CUBOS CENTRALIZADOS
    // O cubo na posição 0 vai de -2 a +2.
    // Somamos metade (2.0) para que -2 vire 0.
    float offset = BLOCO_TAMANHO / 2.0f;
    
    // floor() garante funcionamento com números negativos
    int gridX = (int)floor((x + offset) / BLOCO_TAMANHO);
    int gridZ = (int)floor((z + offset) / BLOCO_TAMANHO);

    char bloco = gMap.getBlock(gridX, gridZ);

    // Debug: Se bugar, tire o // da linha abaixo
    // printf("Pos(%.2f, %.2f) -> Grid[%d][%d] = '%c'\n", x, z, gridZ, gridX, bloco);

    if (bloco == '1') return true; 
    return false; 
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': keyW = true; break;
        case 's': case 'S': keyS = true; break;
        case 'a': case 'A': keyA = true; break;
        case 'd': case 'D': keyD = true; break;
        case 27: exit(0); break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': keyW = false; break;
        case 's': case 'S': keyS = false; break;
        case 'a': case 'A': keyA = false; break;
        case 'd': case 'D': keyD = false; break;
    }
    if ((key == 13 || key == '\r') && (glutGetModifiers() & GLUT_ACTIVE_ALT)) {
        altFullScreen();
    }
}

void atualizaMovimento() {
    float radYaw = yaw * M_PI / 180.0f;
    float dirX = std::sin(radYaw);
    float dirZ = -std::cos(radYaw);
    float strafeX = dirZ; 
    float strafeZ = -dirX;

    float dx = 0.0f;
    float dz = 0.0f;

    if (keyW) { dx += dirX * VELOCIDADE;    dz += dirZ * VELOCIDADE; }
    if (keyS) { dx -= dirX * VELOCIDADE;    dz -= dirZ * VELOCIDADE; }
    if (keyA) { dx += strafeX * VELOCIDADE; dz += strafeZ * VELOCIDADE; }
    if (keyD) { dx -= strafeX * VELOCIDADE; dz -= strafeZ * VELOCIDADE; }

    if (dx == 0.0f && dz == 0.0f) return;

    float proximoX = camX + dx;
    float proximoZ = camZ + dz;

    // Colisão rígida nos 4 cantos do jogador
    bool colidiu = false;
    if (verificaParede(proximoX + PLAYER_RAIO, proximoZ)) colidiu = true; 
    if (verificaParede(proximoX - PLAYER_RAIO, proximoZ)) colidiu = true;
    if (verificaParede(proximoX, proximoZ + PLAYER_RAIO)) colidiu = true; 
    if (verificaParede(proximoX, proximoZ - PLAYER_RAIO)) colidiu = true; 

    if (!colidiu) {
        camX = proximoX;
        camZ = proximoZ;
    }
}

void mouseMotion(int x, int y) {
    if (ignoreWarp) { ignoreWarp = false; return; }
    if (firstMouse) { firstMouse = false; ignoreWarp = true; glutWarpPointer(centerX, centerY); return; }
    int dx = x - centerX; int dy = y - centerY;
    float sens = 0.1f;
    yaw += dx * sens; pitch -= dy * sens;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    ignoreWarp = true; glutWarpPointer(centerX, centerY);
    glutPostRedisplay();
}