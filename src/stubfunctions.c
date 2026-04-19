/* stubfunctions.c */
/* Author(s):  */
/* Description:  */
/* History:

  * 4/19/2026:  Created file

*/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "moves.h"


int legalMove(MOVE *playerMove, PIECE *board[RANKS][FILES], int playerNum, MOVELIST *logMoveList){
        return 1;
}

int castling(MOVE *playerMove, PIECE *board[RANKS][FILES], MOVELIST *logMoveList){
        return 0;
}
int enPassant(MOVE *playerMove, PIECE *board[RANKS][FILES], MOVELIST *logMoveList){
        return 0;
}

/* MOVE computerMove(PIECE *board[RANKS][FILES], int playerNum){
        MOVE compMove = {{0,0}, {0,1}};
        return compMove;
} */

MOVELIST *possibleMoves(POS currentPos, PIECE *board[RANKS][FILES], MOVELIST *logMoves, int playerNum){
        return NULL;
}
void deleteMoveList(MOVELIST *piecePossibleMoves){
        return;
}
int logMove(MOVE *m){
        printf("Logging move.\n");
        return 1;
}
