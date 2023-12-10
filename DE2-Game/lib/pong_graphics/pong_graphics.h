#ifndef PONG_GRAPHICS_H
#define PONG_GRAPHICS_H

/**
 * @file 
 * @defgroup pong Pong graphics library <pong_graphics.h>
 * @code #include <pong_graphics.h> @endcode
 *
 * @brief Pong graphics library.
 *
 * The library contains functions for drawing elements to play the Pong game.
 *
 * @author Matej Hrdlicka, Samuel Blecha, Dept. of Radio Electronics, Brno University 
 *         of Technology, Czechia
 * @copyright (c) 2023 Matej Hrdlicka, Samuel Blecha, This work is licensed under 
 *                the terms of the MIT license
 * @{
 */
#include <inttypes.h>
#include <oled.h>

#define PADDLE_SIZE 10  /// Size of a paddle
#define BALL_SIZE 2     /// Ball size
#define PADDLE_OFFSET 4 /// Offset of a paddle from the border of the screen
#define PADDLE_WIDTH 1  /// Width of a paddle

/**
 * @brief Draws the paddle on the screen
 * Draws paddle for the selected player at selected y coordinate
 * @param player Player to draw the paddle for
 * @param y Height at which is the paddle drawn
 * @return none
 */
extern void drawPaddle(uint8_t player, uint8_t y);

/**
 * @brief Erases the paddle on the screen
 * Erases paddle for the selected player at selected y coordinate
 * @param player Player to erase the paddle for
 * @param y Height at which is the paddle erased
 * @return none
 */
extern void erasePaddle(uint8_t player, uint8_t y);

/**
 * @brief Draws the ball
 * @param x X coordinate of the ball
 * @param y Y coordinate of the ball
 * @return none
 */
extern void drawBall(uint8_t x, uint8_t y);

/**
 * @brief Erases the ball
 * @param x X coordinate of the ball
 * @param y Y coordinate of the ball
 * @return none
 */
extern void eraseBall(uint8_t x, uint8_t y);

/**
 * @brief Displays the score
 * 
 * @param score1 Score of player 1
 * @param score2 Score of player 2
 * @return none
 */
extern void displayScore(uint8_t score1, uint8_t score2);

/**
 * @brief Displays who won
 * 
 * @param player Winner
 * @return none
 */
extern void playerWon(uint8_t player);

/**
 * @brief Displays text after the round ended
 * @return none
 */
extern void ballOut();

/**
 * @brief Displays the main menu
 * @return none
 */
extern void displayMenu();

/** @} */

#endif