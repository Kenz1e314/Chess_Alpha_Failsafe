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

    for (y = RANKS-1; y >= 0; y--) {
        for (x = 0; x < FILES; x++) {
            PIECE *piece = board[y][x];
            if (piece == NULL) {
                continue;
            }

            if (getPieceColor(piece) == Black && getPieceType(piece) == Ant) {
                if (y-1 >= 0 && board[y-1][x] == NULL) {
                    m.pos1.x = x;
                    m.pos1.y = y;
                    m.pos2.x = x;
                    m.pos2.y = y-1;
                    return m;
                }
            }
        }
    }
    return m;
}
