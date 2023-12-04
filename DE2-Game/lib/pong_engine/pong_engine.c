#include "pong_engine.h"

uint8_t directionX = 1;
uint8_t directionY = 1;
uint8_t ballPosX = 63;
uint8_t ballPosY = 31;
uint8_t ballSpeed = 4;
int8_t paddle1Pos = 28;
int8_t paddle2Pos = 28;
uint8_t paddleSpeed = 4;
uint8_t isBehindPaddle = 0;
uint8_t isGameOver = 0;
uint8_t score1 = 0;
uint8_t score2 = 0;

void borderCollision()
{
    if(ballPosY <= BALL_SIZE)
    {
        ballPosY = BALL_SIZE;
        directionY = -directionY;
    }
    else if(ballPosY >= DISPLAY_HEIGHT - 2 - BALL_SIZE)
    {
        ballPosY = DISPLAY_HEIGHT - 2 - BALL_SIZE;
        directionY = -directionY;
    }
    if(isBehindPaddle)
    {
        if(ballPosX <= BALL_SIZE)
        {          
            directionX = -1;
        }
        else if(ballPosX >= DISPLAY_WIDTH - BALL_SIZE - 1)
        {
            directionX = 1;
        }
    }    
}

void paddleCollision()
{
    if(ballPosX <= PADDLE_OFFSET + BALL_SIZE + 3)
    {
        if(ballPosY >= paddle1Pos && ballPosY <= paddle1Pos + PADDLE_SIZE + 1)
        {
            ballPosX = PADDLE_OFFSET + BALL_SIZE + 3;
            directionX = -directionX;
        }
        else
        {
            isBehindPaddle = 1;
            score2++;            
            isGameOver = 1;
        }
    }
    else if(ballPosX >= DISPLAY_WIDTH - 3 - PADDLE_OFFSET - BALL_SIZE)
    {
        if(ballPosY >= paddle2Pos && ballPosY <= paddle2Pos + PADDLE_SIZE + 1)
        {
            ballPosX = DISPLAY_WIDTH - 3 - PADDLE_OFFSET - BALL_SIZE;
            directionX = -directionX;
        }
        else
        {
            isBehindPaddle = 1;
            score1++;
            isGameOver = 1;
        }
    }
}

void calcBallPos()
{
    for(int8_t i = 0; i < ballSpeed; i++)
    {
        borderCollision();
        if(!isBehindPaddle)paddleCollision();
        ballPosX += directionX;
        ballPosY += directionY;
    }
}

void reset()
{   
    init();
    eraseBall(ballPosX, ballPosY);
    directionY = -directionY;
    ballPosX = 63;
    ballPosY = 31;
    paddle1Pos = 28;
    paddle2Pos = 28;
    isBehindPaddle = 0;
    isGameOver = 0;
}

void init()
{
    char string[2];

    oled_clrscr();

    oled_drawLine(63, 0, 63, 63, WHITE);
    oled_gotoxy(8, 0);
    itoa(score1, string, 10);
    oled_puts(string);
    oled_gotoxy(12, 0);
    itoa(score2, string, 10);
    oled_puts(string);

    drawPaddle(0, paddle1Pos);
    drawPaddle(1, paddle2Pos);
    drawBall(ballPosX, ballPosY);

    // Copy buffer to display RAM
    oled_display();
}