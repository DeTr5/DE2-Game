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
#include "pong_graphics.h"


/* Global variables --------------------------------------------------*/
uint8_t directionX = 1;
uint8_t directionY = -1;
uint8_t ballPosX = 20;
uint8_t ballPosY = 40;
float ballSpeed = 1;

// OLED address
#define SENSOR_ADR 0x3c

/* Function definitions ----------------------------------------------*/
/**********************************************************************
* Function: Main function where the program execution begins
* Purpose:  
* Returns:  none
**********************************************************************/
int main(void)
{
    
    // TWI
    twi_init();

    // UART
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    sei();  // Needed for UART

    // Test if sensor is ready
    if (twi_test_address(SENSOR_ADR) == 0)
        uart_puts("OLED display detected\r\n");
    else {
        uart_puts("[ERROR] OLED display not detected\r\n");
        while (1);
    }

    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_drawLine(63, 0, 63, 63, WHITE);

    drawPaddle(0, 20);
    drawPaddle(1, 30);
    drawBall(ballPosX, ballPosY);

    // Copy buffer to display RAM
    oled_display();

    // Timer1
    TIM1_OVF_1SEC
    TIM1_OVF_ENABLE

    sei();

    // Infinite loop
    while (1) {
        eraseBall(ballPosX, ballPosY);
        calcBallPos();
        drawBall(ballPosX, ballPosY);
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
* Function: Timer/Counter1 overflow interrupt
* Purpose:  Read temperature and humidity from DHT12, SLA = 0x5c.
**********************************************************************/
ISR(TIMER1_OVF_vect)
{
    
}


void calcBallPos()
{
    ballPosX += directionX * ballSpeed;
    ballPosY += directionY * ballSpeed;
}