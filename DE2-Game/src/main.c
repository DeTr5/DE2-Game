/***********************************************************************
 * 
 * The I2C (TWI) bus scanner tests all addresses and detects devices
 * that are connected to the SDA and SCL signals.
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
// Declaration of "dht12" variable with structure "DHT_values_structure"
struct DHT_values_structure {
   uint8_t hum_int;
   uint8_t hum_dec;
   uint8_t temp_int;
   uint8_t temp_dec;
   uint8_t checksum;
} dht12;

// Flag for printing new data from sensor
volatile uint8_t new_sensor_data = 0;


// Slave and internal addresses of temperature/humidity sensor DHT12
#define SENSOR_ADR 0x3c
#define SENSOR_HUM_MEM 0
#define SENSOR_TEMP_MEM 2
#define SENSOR_CHECKSUM 4


/* Function definitions ----------------------------------------------*/
/**********************************************************************
* Function: Main function where the program execution begins
* Purpose:  Wait for new data from the sensor and sent them to UART.
* Returns:  none
**********************************************************************/
int main(void)
{
    char string[2];  // String for converting numbers by itoa()

    // TWI
    twi_init();

    // UART
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    sei();  // Needed for UART

    // Test if sensor is ready
    if (twi_test_address(SENSOR_ADR) == 0)
        uart_puts("I2C sensor detected\r\n");
    else {
        uart_puts("[ERROR] I2C device not detected\r\n");
        while (1);
    }

    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_drawLine(63,0,63,63,WHITE);

    drawPaddle(0,20);
    drawPaddle(1,30);
    drawBall(20,40);

    // Copy buffer to display RAM
    oled_display();

    // Timer1
    TIM1_OVF_1SEC
    TIM1_OVF_ENABLE

    sei();

    // Infinite loop
    while (1) {

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
    // Test ACK from sensor
    twi_start();
    if (twi_write((SENSOR_ADR<<1) | TWI_WRITE) == 0) {
        // Set internal memory location
        twi_write(SENSOR_HUM_MEM);
        twi_stop();
        // Read data from internal memory
        twi_start();
        twi_write((SENSOR_ADR<<1) | TWI_READ);
        dht12.hum_int = twi_read(TWI_ACK);
        dht12.hum_dec = twi_read(TWI_ACK);
        dht12.temp_int = twi_read(TWI_ACK);
        dht12.temp_dec = twi_read(TWI_NACK);
        
        new_sensor_data = 1;
    }
    twi_stop();
}