#include "pong_graphics.h"
#include <oled.h>

void drawPaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(4, y, 6, y + PADDLE_SIZE, WHITE);
        break;
    case 1:
        oled_fillRect(121, y, 123, y + PADDLE_SIZE, WHITE);
        break;
    }
}

void drawBall(uint8_t x, uint8_t y)
{
    oled_fillCircle(x, y, BALL_SIZE, WHITE);
}