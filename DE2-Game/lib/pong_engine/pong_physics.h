#ifndef PONG_ENGINE
#define PONG_ENGINE

/**
 * @file 
 * @defgroup pong Pong physics library <pong_physics.h>
 * @code #include <pong_physics.h> @endcode
 *
 * @brief Pong physics library.
 *
 * The library contains functions for calculating physics needed to play the Pong game.
 *
 * @author Matej Hrdlicka, Samuel Blecha, Dept. of Radio Electronics, Brno University 
 *         of Technology, Czechia
 * @copyright (c) 2023 Matej Hrdlicka, Samuel Blecha, This work is licensed under 
 *                the terms of the MIT license
 * @{
 */
#include <inttypes.h>
#include "pong_graphics.h"

/**
 * @brief Detects if there is a collision with the border (only takes in the y coordinate)
 * 
 * @param ballPosY Ball position - y
 * @param directionY Ball direction - y
 * @return Bool return - true if there is a collision
 */
int8_t borderCollision(uint8_t ballPosY, int8_t *directionY);

/**
 * @brief Detects if there is a collision with a paddle of the selected player
 * 
 * @param player Selected player
 * @param ballPosX Ball position - x
 * @param ballPosY Ball position - y
 * @param paddlePos Paddle position of the selected player
 * @param directionX Ball direction - x
 * @param isBehindPaddle If the ball is behind the paddle
 * @return Bool return - true if there is a collision
 */
int8_t paddleCollision(uint8_t player, uint8_t ballPosX, uint8_t ballPosY, uint8_t paddlePos, int8_t *directionX, uint8_t *isBehindPaddle);

/**
 * @brief Calculates the ball position
 * 
 * @param directionX Direction of the ball - x
 * @param directionY Direction of the ball - y
 * @param ballPosX Ball position - x
 * @param ballPosY Ball position - y
 * @return none
 */
void calcBallPos(int8_t directionX, int8_t directionY, uint8_t *ballPosX, uint8_t *ballPosY);

/** @} */

#endif