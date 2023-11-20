#ifndef PONG_GRAPHICS_H
#define PONG_GRAPHICS_H

#include <inttypes.h>

#define PADDLE_SIZE 10
#define BALL_SIZE 2

extern void drawPaddle(uint8_t player, uint8_t y);
extern void drawBall(uint8_t x, uint8_t y);
extern void eraseBall(uint8_t x, uint8_t y);

#endif