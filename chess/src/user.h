/* user.h */
/* Description:  */
/* Author(s):  */
/* History:  
        * 4/14/2026:  Created first draft of user.h (commented out parseMove())

*/


#ifndef USER_H
#define USER_H

#include "chess.h"

//#ifdef TESTING
#include "moves.h"
//#endif

/* void printMenu(void):  Prints the main menu */
void printMenu(void);

/* void printBoard(PIECE *board[RANKS][FILES]):  Prints the board's current state */
void printBoard(PIECE *board[RANKS][FILES]);

/* void helpMenu(void):  Prints the help menu */
void helpMenu(void);

/* void endGame(int isCheckmate, int isStalemate):  Indicates stalemate, player 1 in checkmate (player 2 wins), or player 2 in checkmate (player 1 wins) */
void endGame(int isCheckmate, int isStalemate);

MOVE parseMove(char *inputStr);  // not put in right now because it relies on Logging module



#endif

