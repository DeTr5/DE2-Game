/***********************************************************************
 * 
 * 
 * 
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2023 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions
#include <oled.h>
#include <gpio.h>
#include "pong_physics.h"

//Game Parameters
#define  STARTING_BALL_POS_X 20;
#define  STARTING_BALL_POS_Y 40;
#define  DEFAULT_BALL_SPEED 4;
#define  DEFAULT_PADDLE_POS 30;
#define  DEFAULT_PADDLE_SPEED 4;
#define  PLAYER_ONE 0;
#define  PLAYER_TWO 1;
#define  DELAY_BETWEEN_GAMES 5; //in fames

// OLED address
#define OLED_ADR 0x3c

//Buttons
#define PADDLE1_UP 4
#define PADDLE1_DOWN 5
#define PADDLE2_UP 6
#define PADDLE2_DOWN 7

/* Global variables --------------------------------------------------*/

int8_t directionX = 1;
int8_t directionY = -1;
uint8_t ballPosX = STARTING_BALL_POS_X;
uint8_t ballPosY = STARTING_BALL_POS_Y;
uint8_t ballSpeed = DEFAULT_BALL_SPEED;
int8_t paddle1Pos = DEFAULT_PADDLE_POS;
int8_t paddle2Pos = DEFAULT_PADDLE_POS;
uint8_t paddleSpeed = DEFAULT_PADDLE_SPEED;
uint8_t isBehindPaddle = 0;
uint8_t isGameOver = 0;
uint8_t score1 = 0;
uint8_t score2 = 0;
uint8_t bounces = 0;
uint8_t gameOverDelayCounter = 5;

/* Function definitions ----------------------------------------------*/
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
    bounces = 0;
    gameOverDelayCounter = 5;

    oled_drawLine(63, 0, 63, 63, WHITE);
    displayScore(score1, score2);

    drawPaddle(0, paddle1Pos);
    drawPaddle(1, paddle2Pos);
    drawBall(ballPosX, ballPosY);

    // Copy buffer to display RAM
    oled_display();
}

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

void keyboardInput()
{
    if(uart_getc() == 's' && paddle1Pos >= 0)
    {
        erasePaddle(0, paddle1Pos);
        if(paddle1Pos - paddleSpeed <= 0) paddle1Pos = 0;
        else paddle1Pos -= paddleSpeed;
        drawPaddle(0, paddle1Pos);
    }
    /*if(!GPIO_read(&PINB, PADDLE1_DOWN) && paddle1Pos <= DISPLAY_HEIGHT-2-PADDLE_SIZE)
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
    }*/
} 

/**********************************************************************
* Function: Main function where the program execution begins
* Purpose:  
* Returns:  none
**********************************************************************/
int main(void)
{
    GPIO_mode_input_pullup(&DDRB, PADDLE1_UP);
    GPIO_mode_input_pullup(&DDRB, PADDLE1_DOWN);
    GPIO_mode_input_pullup(&DDRB, PADDLE2_UP);
    GPIO_mode_input_pullup(&DDRB, PADDLE2_DOWN);

    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
    // Timer0
    TIM0_OVF_16MS
    TIM0_OVF_ENABLE

    // Timer1
    /*TIM1_OVF_33MS
    TIM1_OVF_ENABLE*/
    
    // TWI
    twi_init();

    oled_init(OLED_DISP_ON);
    reset();

    sei();

    // Infinite loop
    while (1) {}

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
* Function: Timer/Counter1 overflow interrupt
* Purpose:  
**********************************************************************/
ISR(TIMER0_OVF_vect)
{
    if(!isGameOver)
    {
        eraseBall(ballPosX, ballPosY);
        paddleInput();
        keyboardInput();
        for(uint8_t i = 0; i < ballSpeed; i++)
        {
            borderCollision(ballPosY, &directionY);
            if(!isBehindPaddle)
            {
                if(paddleCollision(0, ballPosX, ballPosY, paddle1Pos, &directionX, &isBehindPaddle)) bounces++;
                if(paddleCollision(1, ballPosX, ballPosY, paddle2Pos, &directionX, &isBehindPaddle)) bounces++;
            }
            else
            {
                isGameOver = 1;
                if(ballPosX < 32) score2++;
                else score1++;
            }
            calcBallPos(directionX, directionY, &ballPosX, &ballPosY);
        }
        drawBall(ballPosX, ballPosY);
        oled_drawLine(63, 0, 63, 63, WHITE);
        displayScore(score1, score2);
    }

    if (isGameOver)
    {
        switch (gameOverDelayCounter)
        {
        case 1:
            oled_gotoxy(2, 3);
            oled_puts("Press any button");
            oled_gotoxy(5, 4);
            oled_puts("to reset");
            gameOverDelayCounter--;
            break;

        case 0:
            //if(GPIO_read(&PINB, PADDLE1_UP) || GPIO_read(&PINB, PADDLE1_DOWN) || GPIO_read(&PINB, PADDLE2_UP) || GPIO_read(&PINB, PADDLE2_DOWN)) reset();
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
    oled_display();
}

/*ISR(TIMER1_OVF_vect)
{
    
}*/