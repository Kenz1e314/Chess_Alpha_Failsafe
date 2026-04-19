/* stubfunctions.h */
/* Author(s):  */
/* Description */
/* History:
    * 4/19/2026:  Created file
*/


#ifndef STUBFUNCTIONS_H
#define STUBFUNCTIONS_H

// gamestate.c
int legalMove(MOVE *playerMove, PIECE *board[RANKS][FILES], int playerNum, MOVELIST *logMoveList);
MOVE computerMove(PIECE *board[RANKS][FILES], int playerNum);
int castling(MOVE *playerMove, PIECE *board[RANKS][FILES], MOVELIST *logMoveList);
int enPassant(MOVE *playerMove, PIECE *board[RANKS][FILES], MOVELIST *logMoveList);
MOVELIST *possibleMoves(POS currentPos, PIECE *board[RANKS][FILES], MOVELIST *logMoves, int playerNum);
void deleteMoveList(MOVELIST *piecePossibleMoves);
int logMove(MOVE *m);


#endif
