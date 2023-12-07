/***********************************************************************
 * 
 * 
 * 
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2023 Matej Hrdlicka, Samuel Blecha
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         /// AVR device-specific IO definitions
#include <avr/interrupt.h>  /// Interrupts standard C library for AVR-GCC
#include "timer.h"          /// Timer library for AVR-GCC
#include <twi.h>            /// I2C/TWI library for AVR-GCC
#include <uart.h>           /// Peter Fleury's UART library
#include <stdlib.h>         /// C library. Needed for number conversions
#include <oled.h>           /// Oled library
#include <gpio.h>           /// Gpio library
#include "pong_physics.h"   /// Library with physics calculations for the game. Includes pong_graphics.h

//Game Parameters
#define  STARTING_BALL_POS_X 20 /// Starting ball position - x
#define  STARTING_BALL_POS_Y 40 /// Starting ball position - y
#define  DEFAULT_BALL_SPEED 4   /// Starting ball speed
#define  DEFAULT_PADDLE_POS 30  /// Starting paddle position
#define  DEFAULT_PADDLE_SPEED 4 /// Starting paddle speed
#define  PLAYER_ONE 0           /// Defines first player as 0
#define  PLAYER_TWO 1           /// Defines second player as 1
#define  DELAY_BETWEEN_GAMES 5  /// Draw number of frames before the round ends
#define  BOUNCES_FOR_SPEEDUP 5  /// Bounces needed to speedup the ball
#define  POINTS_TO_WIN 5        /// Points needed to win


/**Game States
 * Used to define states of the game
 */
#define  IN_GAME 0
#define  MAIN_MENU 1

#define OLED_ADR 0x3c   ///Oled address

/**Buttons
 * Buttons defines
 */
#define PADDLE1_UP 0
#define PADDLE1_DOWN 1
#define PADDLE2_UP 2
#define PADDLE2_DOWN 3

/* Global variables --------------------------------------------------*/

int8_t directionX = 1;                              /// Direction of ball movement - x
int8_t directionY = -1;                             /// Direction of ball movement - y
uint8_t ballPosX = STARTING_BALL_POS_X;             /// Ball position - x
uint8_t ballPosY = STARTING_BALL_POS_Y;             /// Ball position - y
uint8_t ballSpeed = DEFAULT_BALL_SPEED;             /// Ball speed
int8_t paddle1Pos = DEFAULT_PADDLE_POS;             /// Position of player 1
int8_t paddle2Pos = DEFAULT_PADDLE_POS;             /// Position of player 2
uint8_t paddleSpeed = DEFAULT_PADDLE_SPEED;         /// Paddle speed
uint8_t isBehindPaddle = 0;                         /// Bool variable - true if the ball is behind any paddle
uint8_t isGameOver = 0;                             /// Bool variable - true if the ball colided with the border behind any paddle
uint8_t score1 = 0;                                 /// Score of player 1
uint8_t score2 = 0;                                 /// Score of player 2
uint8_t bounces = 1;                                /// Number of times the ball bounced
uint8_t gameOverDelayCounter = DELAY_BETWEEN_GAMES; /// Counter to draw frames after the round ends
uint8_t gameState = 1;                              /// State of the game
uint8_t isWinner = 0;                               /// Bool variable - true if somebody has won

/* Function definitions ----------------------------------------------*/
/**
 * @brief Round reset function
 * Used to reset the game after the round ends or to initialize the game
 * @return none
 */
void reset()
{   
    oled_clrscr();
    directionY = -directionY;
    ballPosX = 63;
    ballPosY = 31;
    ballSpeed = DEFAULT_BALL_SPEED;
    paddle1Pos = DEFAULT_PADDLE_POS;
    paddle2Pos = DEFAULT_PADDLE_POS;
    isBehindPaddle = 0;
    bounces = 1;
    gameOverDelayCounter = DELAY_BETWEEN_GAMES;
    isGameOver = 0;
    isWinner = 0;

    oled_drawLine(63, 0, 63, 63, WHITE);
    displayScore(score1, score2);

    drawPaddle(0, paddle1Pos);
    drawPaddle(1, paddle2Pos);
    drawBall(ballPosX, ballPosY);

    // Copy buffer to display RAM
    oled_display();
}

/**
 * @brief Function to detect pressed buttons
 * @return none
 */
void paddleInput()
{
    if(!GPIO_read(&PINB, PADDLE1_UP) && paddle1Pos >= 0)
    {
        erasePaddle(0, paddle1Pos);
        if(paddle1Pos - paddleSpeed <= 0) paddle1Pos = 0;
        else paddle1Pos -= paddleSpeed;
        drawPaddle(0, paddle1Pos);
    }
    if(!GPIO_read(&PINB, PADDLE1_DOWN) && paddle1Pos <= DISPLAY_HEIGHT-2-PADDLE_SIZE)
    {
        erasePaddle(0, paddle1Pos);
        if(paddle1Pos + paddleSpeed >= DISPLAY_HEIGHT-2-PADDLE_SIZE) paddle1Pos = DISPLAY_HEIGHT-2-PADDLE_SIZE;
        else paddle1Pos += paddleSpeed;
        drawPaddle(0, paddle1Pos);
    }
    if(!GPIO_read(&PINB, PADDLE2_UP) && paddle2Pos >= 0)
    {
        erasePaddle(1, paddle2Pos);
        if(paddle2Pos - paddleSpeed <= 0) paddle2Pos = 0;
        else paddle2Pos -= paddleSpeed;
        drawPaddle(1, paddle2Pos);
    }
    if(!GPIO_read(&PINB, PADDLE2_DOWN) && paddle2Pos <= DISPLAY_HEIGHT-2-PADDLE_SIZE)
    {
        erasePaddle(1, paddle2Pos);
        if(paddle2Pos + paddleSpeed >= DISPLAY_HEIGHT-2-PADDLE_SIZE) paddle2Pos = DISPLAY_HEIGHT-2-PADDLE_SIZE;
        else paddle2Pos += paddleSpeed;
        drawPaddle(1, paddle2Pos);
    }
}

/**********************************************************************
* Function: Main function where the program execution begins
* Purpose:  Initializes the application
* Returns:  none
**********************************************************************/
int main(void)
{
    GPIO_mode_input_pullup(&DDRB, PADDLE1_UP);
    GPIO_mode_input_pullup(&DDRB, PADDLE1_DOWN);
    GPIO_mode_input_pullup(&DDRB, PADDLE2_UP);
    GPIO_mode_input_pullup(&DDRB, PADDLE2_DOWN);
    
    // Timer0
    TIM1_OVF_33MS
    TIM1_OVF_ENABLE

    // Timer1
    /*TIM1_OVF_33MS
    TIM1_OVF_ENABLE*/
    
    // TWI
    twi_init();

    oled_init(OLED_DISP_ON);

    sei();

    // Infinite loop
    while (1) {}

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
* Function: Timer/Counter1 overflow interrupt
* Purpose:  Whole code is here, main switch is used to switch between game states
**********************************************************************/
ISR(TIMER1_OVF_vect)
{
    switch (gameState)
    {
    case MAIN_MENU:
        displayMenu();
        if(!GPIO_read(&PINB, PADDLE1_UP) || !GPIO_read(&PINB, PADDLE1_DOWN) || !GPIO_read(&PINB, PADDLE2_UP) || !GPIO_read(&PINB, PADDLE2_DOWN))
        {
            gameState = IN_GAME;
            reset();
        }
        break;

    case IN_GAME:
        if(!isGameOver)
        {
            eraseBall(ballPosX, ballPosY);
            paddleInput();
            for(uint8_t i = 0; i < ballSpeed; i++)
            {
                borderCollision(ballPosY, &directionY);
                if(!isBehindPaddle)
                {
                    if(paddleCollision(0, ballPosX, ballPosY, paddle1Pos, &directionX, &isBehindPaddle))
                    {
                        bounces++;
                        if(bounces % BOUNCES_FOR_SPEEDUP == 0) ballSpeed++;           
                    } 
                    if(paddleCollision(1, ballPosX, ballPosY, paddle2Pos, &directionX, &isBehindPaddle))
                    {
                        bounces++;
                        if(bounces % BOUNCES_FOR_SPEEDUP == 0) ballSpeed++;
                    }
                }
                else
                {
                    isGameOver = 1;
                    i = ballSpeed;
                    if(ballPosX < 32) score2++;
                    else score1++;
                }
                calcBallPos(directionX, directionY, &ballPosX, &ballPosY);
            }
            drawBall(ballPosX, ballPosY);
            oled_drawLine(63, 0, 63, 63, WHITE);
            displayScore(score1, score2);        
        }
        else
        {
            switch (gameOverDelayCounter)
            {
            case 1:
                if(score1 == POINTS_TO_WIN || score2 == POINTS_TO_WIN)
                {
                    isWinner = 1;
                    if(score1 == POINTS_TO_WIN)
                    {
                        playerWon(1);
                    }
                    else
                    {
                        playerWon(2);
                    }
                    gameOverDelayCounter--;
                }
                else
                {
                    ballOut();
                    gameOverDelayCounter--;
                }
                break;
    
            case 0:
                if(!GPIO_read(&PINB, PADDLE1_UP) || !GPIO_read(&PINB, PADDLE1_DOWN) || !GPIO_read(&PINB, PADDLE2_UP) || !GPIO_read(&PINB, PADDLE2_DOWN))
                {                                
                    if(isWinner)
                    {
                        score1 = 0;
                        score2 = 0;
                        gameState = MAIN_MENU;
                        oled_clrscr();
                    } 
                    else reset();
                } 
                break;
            
            default:
                eraseBall(ballPosX, ballPosY);
                for (uint8_t i = 0; i < ballSpeed; i++)
                {
                    borderCollision(ballPosY, &directionY);                
                    calcBallPos(directionX, directionY, &ballPosX, &ballPosY);
                }
                drawBall(ballPosX, ballPosY);
                oled_drawLine(63, 0, 63, 63, WHITE);
                displayScore(score1, score2);
                gameOverDelayCounter--;
                break;
            }
        }
        break;
    
    default:
        break;
    }
    oled_display();
}