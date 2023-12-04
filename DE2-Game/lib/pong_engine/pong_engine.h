#ifndef PONG_ENGINE
#define PONG_ENGINE

#include <inttypes.h>
#include "pong_graphics.h"

extern uint8_t directionX;
extern uint8_t directionY;
extern uint8_t ballPosX;
extern uint8_t ballPosY;
extern uint8_t ballSpeed;
extern int8_t paddle1Pos;
extern int8_t paddle2Pos;
extern uint8_t paddleSpeed;
extern uint8_t isBehindPaddle;
extern uint8_t isGameOver;
extern uint8_t score1;
extern uint8_t score2;
extern uint8_t bounces;

extern void borderCollision();
extern void paddleCollision();
extern void calcBallPos();
extern void reset();
extern void displayScore();

#endif