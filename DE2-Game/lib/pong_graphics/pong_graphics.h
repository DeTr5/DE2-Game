#ifndef PONG_GRAPHICS_H
#define PONG_GRAPHICS_H

#include <inttypes.h>
#include <oled.h>

#define PADDLE_SIZE 10
#define BALL_SIZE 2
#define PADDLE_OFFSET 4
#define PADDLE_WIDTH 1

extern void drawPaddle(uint8_t player, uint8_t y);
extern void erasePaddle(uint8_t player, uint8_t y);
extern void drawBall(uint8_t x, uint8_t y);
extern void eraseBall(uint8_t x, uint8_t y);
extern void displayScore(uint8_t score1, uint8_t score2);
extern void playerWon(uint8_t player);
extern void ballOut();
extern void displayMenu();

#endif