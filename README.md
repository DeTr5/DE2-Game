# Simple arcade game - DE2 project

### Team members

* Matěj Hrdlička
* Samuel Blecha

## Theoretical description and explanation

The goal of the project was to create a simple arcade game - in our case the Pong game. This Pong game is for 2 players. The game is played on OLED display and each player has two buttons. The goal of the game is to move your paddle up or down to bounce the ball to the other side of the screen where the other player has his paddle. If you miss the ball, your oponent gets a point. If you collect 5 points, you win.

## Hardware description of demo application

The OLED display is connected to Arduino using UART. The buttons are connected on port B to the pins 0 - 3.

## Software description

Libraries:

* gpio - used to access values on digital pins
* oled - writes and draws on the OLED display - includes twi and uart libraries
* pong_engine - calculates physics of the game and draws game objects on the screen, includes pong_graphics library

The main code is in timer1 overflow function. This is because of the need to run the game in a constant framerate.

## Instructions

Only the four buttons are needed to operate the game. In menu, simply press any button to start a new game. After a round you need to just press any button to start the next round.

https://drive.google.com/file/d/1IPwJwsJ_EZ-hXsV55nXo5yBYHyMl_1Y9/view?usp=drivesdk
