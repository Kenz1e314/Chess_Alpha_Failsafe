/* computer.c */

#include <stdio.h>
#include "computer.h"
#include "moves.h"

MOVE computerMove(PIECE *board[RANKS][FILES], int playerNum) {
    int x, y;
    MOVE m;

    m.pos1.x = -1;
    m.pos1.y = -1;
    m.pos2.x = -1;
    m.pos2.y = -1;

    if (playerNum != Black) {
        return m;
    }

    for (x = RANKS-1; x >= 0; x--) {
        for (y = 0; y < FILES; y++) {
            PIECE *piece = board[x][y];
            if (piece == NULL) {
                continue;
            }

            if (getPieceColor(piece) == Black && getPieceType(piece) == Ant) {
                if (x-1 >= 0 && board[x-1][y] == NULL) {
                    m.pos1.x = x;
                    m.pos1.y = y;
                    m.pos2.x = x-1;
                    m.pos2.y = y;
                    return m;
                }
            }
        }
    }
    return m;
}
