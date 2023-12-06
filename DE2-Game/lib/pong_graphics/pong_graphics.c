#include "pong_graphics.h"

void drawPaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET + PADDLE_WIDTH, y + PADDLE_SIZE, WHITE);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - PADDLE_WIDTH, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, WHITE);
        break;
    }
}

void erasePaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET + PADDLE_WIDTH, y + PADDLE_SIZE, BLACK);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - PADDLE_WIDTH, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, BLACK);
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

void displayScore(uint8_t score1, uint8_t score2)
{
    char string[2];    
    oled_gotoxy(8, 0);
    itoa(score1, string, 10);
    oled_puts(string);
    oled_gotoxy(12, 0);
    itoa(score2, string, 10);
    oled_puts(string);
}