#ifndef SCENE_H
#define SCENE_H

extern float anguloPiramide;
extern float anguloEsfera;
extern float anguloRoda;

extern float camX, camY, camZ;
extern float yaw, pitch;

void desenhaChao(void);
void desenhaTorresEPiramides(void);
void desenhaPiramideDegraus(void);
void desenhaBicicleta(void);

#endif
