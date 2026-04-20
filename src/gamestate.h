/* GameState.h: header file for gameState.c
* Author: Sanaa Bebal
* Modifications:
* 04/16/2026 SB initial modifications
*/


#ifndef GAMESTATE_H
#define GAMESTATE_H


#include "chess.h"

void makeMove(int playerNum, int isComputer, PIECE *board[RANKS][FILES], MOVELIST *logMoveList);

#endif
