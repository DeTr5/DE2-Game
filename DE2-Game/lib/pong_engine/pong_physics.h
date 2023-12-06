#ifndef PONG_ENGINE
#define PONG_ENGINE

#include <inttypes.h>
#include "pong_graphics.h"

int8_t borderCollision(uint8_t ballPosY, int8_t *directionY);
int8_t paddleCollision(uint8_t player, uint8_t ballPosX, uint8_t ballPosY, uint8_t paddlePos, int8_t *directionX, uint8_t *isBehindPaddle);
void calcBallPos(int8_t directionX, int8_t directionY, uint8_t *ballPosX, uint8_t *ballPosY);

#endif