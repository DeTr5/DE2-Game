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
#include "pong_graphics.h"


/* Global variables --------------------------------------------------*/
uint8_t directionX = 1;
uint8_t directionY = -1;
uint8_t ballPosX = 20;
uint8_t ballPosY = 40;
uint8_t ballSpeed = 2;
uint8_t paddle1Pos = 30;
uint8_t paddle2Pos = 30;
uint8_t isGameOver = 0;

// OLED address
#define SENSOR_ADR 0x3c

//Buttons
#define PADDLE1_UP 0
#define PADDLE1_DOWN 1
#define PADDLE2_UP 2
#define PADDLE2_DOWN 3

/* Function definitions ----------------------------------------------*/
void borderCollision()
{
    if(ballPosY <= BALL_SIZE)
    {
        ballPosY = BALL_SIZE;
        directionY = -directionY;
    }
    else if( ballPosY >= DISPLAY_HEIGHT - 2 - BALL_SIZE)
    {
        ballPosY = DISPLAY_HEIGHT - 2 - BALL_SIZE;
        directionY = -directionY;
    }    
}

void paddleCollision()
{
    if(ballPosX <= PADDLE_OFFSET + BALL_SIZE + 3)
    {
        if(ballPosY >= paddle1Pos && ballPosY <= paddle1Pos + PADDLE_SIZE)
        {
            ballPosX = PADDLE_OFFSET + BALL_SIZE + 3;
            directionX = -directionX;
        }
    }
    else if(ballPosX >= DISPLAY_WIDTH - 3 - PADDLE_OFFSET - BALL_SIZE)
    {
        if(ballPosY >= paddle2Pos && ballPosY <= paddle2Pos + PADDLE_SIZE)
        {
            ballPosX = DISPLAY_WIDTH - 3 - PADDLE_OFFSET - BALL_SIZE;
            directionX = -directionX;
        }
    }
}

void calcBallPos()
{
    borderCollision();
    paddleCollision();
    ballPosX += directionX * ballSpeed;
    ballPosY += directionY * ballSpeed;
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
    
    // Timer0
    TIM0_OVF_16MS
    TIM0_OVF_ENABLE

    // Timer1
    /*TIM1_OVF_33MS
    TIM1_OVF_ENABLE*/
    
    // TWI
    twi_init();

    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_drawLine(63, 0, 63, 63, WHITE);

    drawPaddle(0, paddle1Pos);
    drawPaddle(1, paddle2Pos);
    drawBall(ballPosX, ballPosY);

    // Copy buffer to display RAM
    oled_display();

    sei();

    // Infinite loop
    while (1) {}

    /*oled_clrscr();
    oled_gotoxy(30,60);
    oled_puts("GAME OVER");*/

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
    if(!GPIO_read(&PINB, PADDLE1_UP) && paddle1Pos!=0)
    {
        erasePaddle(0, paddle1Pos);
        paddle1Pos--;
        drawPaddle(0, paddle1Pos);
    }
    if(!GPIO_read(&PINB, PADDLE1_DOWN) && paddle1Pos != DISPLAY_HEIGHT-2-PADDLE_SIZE)
    {
        erasePaddle(0, paddle1Pos);
        paddle1Pos++;
        drawPaddle(0, paddle1Pos);
    }
    if(!GPIO_read(&PINB, PADDLE2_UP) && paddle2Pos!=0)
    {
        erasePaddle(1, paddle2Pos);
        paddle2Pos--;
        drawPaddle(1, paddle2Pos);
    }
    if(!GPIO_read(&PINB, PADDLE2_DOWN) && paddle2Pos != DISPLAY_HEIGHT-2-PADDLE_SIZE)
    {
        erasePaddle(1, paddle2Pos);
        paddle2Pos++;
        drawPaddle(1, paddle2Pos);
    }

    eraseBall(ballPosX, ballPosY);
    calcBallPos();
    drawBall(ballPosX, ballPosY);
    oled_drawLine(63, 0, 63, 63, WHITE);

    oled_display();
}

/*ISR(TIMER1_OVF_vect)
{
    eraseBall(ballPosX, ballPosY);
    calcBallPos();
    drawBall(ballPosX, ballPosY);
    oled_drawLine(63, 0, 63, 63, WHITE);

    oled_display();
}*/