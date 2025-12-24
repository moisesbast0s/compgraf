#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include "drawlevel.h"
#include "scene.h"
#include "utils/levelmetrics.h"
#include <cstdio>

extern GLuint texParede; 
extern GLuint texLava;
extern GLuint texSangue; 
extern GLuint texChao;

extern GLuint progLava;
extern GLuint progSangue;
extern float tempoEsfera;

// Config do grid
static const float TILE = 4.0f;    
static const float WALL_H = 4.0f;  
static const float EPS_Y = 0.001f; 

static void bindTexture0(GLuint tex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
}

static void desenhaQuadChao(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(x - half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, 0.0f);    glVertex3f(x + half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, tilesUV); glVertex3f(x + half, EPS_Y, z - half);
    glTexCoord2f(0.0f, tilesUV);    glVertex3f(x - half, EPS_Y, z - half);
    glEnd();
}

static void desenhaTileChao(float x, float z)
{
    glUseProgram(0); 
    glColor3f(1, 1, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texChao);
    desenhaQuadChao(x, z, TILE, 2.0f);
}

static void desenhaParede(float x, float z)
{
    float half = TILE * 0.5f;
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texParede);

    float tilesX = 1.0f;
    float tilesY = 1.0f; // Ajustei para 1.0 para a textura não ficar esticada demais

    glBegin(GL_QUADS);
    // Frente (z+)
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);     glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);   glVertex3f(x + half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);     glVertex3f(x - half, WALL_H, z + half);

    // Trás (z-)
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);     glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);   glVertex3f(x - half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);     glVertex3f(x + half, WALL_H, z - half);

    // Direita (x+)
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);     glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);   glVertex3f(x + half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);     glVertex3f(x + half, WALL_H, z + half);

    // Esquerda (x-)
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);     glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);   glVertex3f(x - half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);     glVertex3f(x - half, WALL_H, z - half);
    glEnd();
}

static void desenhaTileLava(float x, float z)
{
    glUseProgram(progLava);
    // ... (Mantém seus uniforms de lava) ...
    // Estou simplificando para caber na resposta, mas mantenha seus uniforms aqui
    // Se o shader precisar, copie do seu código original a parte dos uniforms
    
    // Configurações minimas para rodar se você copiar só isso:
    GLint locTime = glGetUniformLocation(progLava, "uTime");
    glUniform1f(locTime, tempoEsfera);
    
    bindTexture0(texLava);
    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);
    glUseProgram(0);
}

static void desenhaTileSangue(float x, float z)
{
    glUseProgram(progSangue);
    GLint locTime = glGetUniformLocation(progSangue, "uTime");
    glUniform1f(locTime, tempoEsfera);
    
    bindTexture0(texSangue);
    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);
    glUseProgram(0);
}

void drawLevel(const MapLoader &map)
{
    const auto &data = map.data();
    int H = map.getHeight();
    int W = map.getWidth();

    // NÃO USAMOS MAIS O LevelMetrics PARA CENTRALIZAR
    // Isso garante que o Bloco[0][0] comece visualmente no World(0,0)

    for (int z = 0; z < H; z++)
    {
        for (int x = 0; x < (int)data[z].size(); x++)
        {
            // CÁLCULO MANUAL (Simples e Alinhado com a Física)
            float wx = x * TILE;
            float wz = z * TILE;

            char c = data[z][x];

            if (c == '0') desenhaTileChao(wx, wz);
            else if (c == '1') desenhaParede(wx, wz);
            else if (c == 'L') desenhaTileLava(wx, wz);
            else if (c == 'B') desenhaTileSangue(wx, wz);
        }
    }
}