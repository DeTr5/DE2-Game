#include "pong_graphics.h"
#include <oled.h>

void drawPaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET+2, y + PADDLE_SIZE, WHITE);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - 2, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, WHITE);
        break;
    }
}

void erasePaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET+2, y + PADDLE_SIZE, BLACK);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - 2, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, BLACK);
        break;
    }
}

void drawBall(uint8_t x, uint8_t y)
{
    oled_fillCircle(x, y, BALL_SIZE, WHITE);
}

void eraseBall(uint8_t x, uint8_t y)
{
    oled_fillCircle(x, y, BALL_SIZE, BLACK);
}