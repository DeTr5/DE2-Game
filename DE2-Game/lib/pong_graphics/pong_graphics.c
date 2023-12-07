#include "pong_graphics.h"

/**********************************************************************
 * Function: drawPaddle()
 * Purpose:  Draws paddle for the selected player at selected y coordinate
 * Input:    Selected player and paddle height
 * Returns:  none
 **********************************************************************/
void drawPaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET + PADDLE_WIDTH, y + PADDLE_SIZE, WHITE);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - PADDLE_WIDTH, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, WHITE);
        break;
    }
}

/**********************************************************************
 * Function: erasePaddle()
 * Purpose:  Erases paddle for the selected player at selected y coordinate
 * Input:    Selected player and paddle height
 * Returns:  none
 **********************************************************************/
void erasePaddle(uint8_t player, uint8_t y)
{
    switch (player)
    {
    case 0:
        oled_fillRect(PADDLE_OFFSET, y, PADDLE_OFFSET + PADDLE_WIDTH, y + PADDLE_SIZE, BLACK);
        break;
    case 1:
        oled_fillRect(DISPLAY_WIDTH - PADDLE_OFFSET - PADDLE_WIDTH, y, DISPLAY_WIDTH - PADDLE_OFFSET, y + PADDLE_SIZE, BLACK);
        break;
    }
}

/**********************************************************************
 * Function: drawBall()
 * Purpose:  Draws the ball
 * Input:    Ball coordinates
 * Returns:  none
 **********************************************************************/
void drawBall(uint8_t x, uint8_t y)
{
    oled_fillCircle(x, y, BALL_SIZE, WHITE);
}

/**********************************************************************
 * Function: eraseBall()
 * Purpose:  Erases the ball
 * Input:    Ball coordinates
 * Returns:  none
 **********************************************************************/
void eraseBall(uint8_t x, uint8_t y)
{
    oled_fillCircle(x, y, BALL_SIZE, BLACK);
}

/**********************************************************************
 * Function: displayScore()
 * Purpose:  Displays the score
 * Input:    Score of both players
 * Returns:  none
 **********************************************************************/
void displayScore(uint8_t score1, uint8_t score2)
{
    char string[2];    
    oled_gotoxy(8, 0);
    itoa(score1, string, 10);
    oled_puts(string);
    oled_gotoxy(12, 0);
    itoa(score2, string, 10);
    oled_puts(string);
}

/**********************************************************************
 * Function: playerWon()
 * Purpose:  Displays who won
 * Input:    Winner
 * Returns:  none
 **********************************************************************/
void playerWon(uint8_t player)
{
    switch (player)
    {
    case 1:
        oled_gotoxy(4, 2);
        oled_puts("PLAYER 1 WON!");
        break;

    case 2:
        oled_gotoxy(4, 2);
        oled_puts("PLAYER 2 WON!");
        break;
    
    default:
        break;
    }
    oled_gotoxy(2, 5);
    oled_puts("Press any button");
    oled_gotoxy(2, 6);
    oled_puts("to return to Menu");
}

/**********************************************************************
 * Function: ballOut()
 * Purpose:  Displays text after the round ended
 * Returns:  none
 **********************************************************************/
void ballOut()
{
    oled_gotoxy(2, 4);
    oled_puts("Press any button");
    oled_gotoxy(6, 5);
    oled_puts("to reset");
}

/**********************************************************************
 * Function: displayMenu()
 * Purpose:  Displays the main menu
 * Returns:  none
 **********************************************************************/
void displayMenu()
{
    oled_gotoxy(6, 0);
    oled_charMode(DOUBLESIZE);
    oled_puts("PONG");
    oled_charMode(NORMALSIZE);
    oled_gotoxy(2, 3);
    oled_puts("Press any button");
    oled_gotoxy(6, 4);
    oled_puts("to START");
}