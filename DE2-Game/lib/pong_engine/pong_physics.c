#include "pong_physics.h"

int8_t borderCollision(uint8_t ballPosY, int8_t *directionY)
{
    if(ballPosY <= BALL_SIZE)
    {
        *directionY = 1;
        return 1;
    }
    else if(ballPosY >= DISPLAY_HEIGHT - 2 - BALL_SIZE)
    {
        *directionY = -1;
        return 1;
    }
    else return 0;    
}

int8_t paddleCollision(uint8_t player, uint8_t ballPosX, uint8_t ballPosY, uint8_t paddlePos, int8_t *directionX, uint8_t *isBehindPaddle)
{
    switch (player)
    {
    case 0:
        if(ballPosX <= PADDLE_OFFSET + BALL_SIZE + PADDLE_WIDTH)
        {
            if(ballPosY >= paddlePos && ballPosY <= paddlePos + PADDLE_SIZE + 1)
            {
                *directionX = 1;
                return 1;
            }
            else
            {
                *isBehindPaddle = 1;
                return 0;
            }
        }
        return 0;
        break;

    case 1:
        if(ballPosX >= DISPLAY_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET - BALL_SIZE -1)
        {
            if(ballPosY >= paddlePos && ballPosY <= paddlePos + PADDLE_SIZE + 1)
            {
                *directionX = -1;
                return 1;
            }
            else
            {
                *isBehindPaddle = 1;
                return 0;
            }
        }
        return 0;
        break;
    
    default:
        return 0;
        break;
    }    
    
    
}

void calcBallPos(int8_t directionX, int8_t directionY, uint8_t *ballPosX, uint8_t *ballPosY)
{
    *ballPosX += directionX;
    *ballPosY += directionY;
}