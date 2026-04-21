/* stubfunctions.c asdf*/
/* Author(s):  */
/* Description:  */
/* History:

  * 4/19/2026:  Created file

*/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chess.h"
#include "moves.h"
#include "stubfunctions.h"


/* gamestate.c calls */
int legalMove(MOVE *playerMove, PIECE *board[RANKS][FILES], int playerNum, MOVELIST *logMoveList){
    if (playerMove == NULL) return 0;

    int x = playerMove->pos1.x;
    int y = playerMove->pos1.y;

    if (x < 0 || x >= RANKS || y < 0 || y >= FILES) return 0;
    if (playerMove->pos2.x < 0 || playerMove->pos2.x >= RANKS ||
        playerMove->pos2.y < 0 || playerMove->pos2.y >= FILES) return 0;

    PIECE *piece = board[x][y];
    if (piece == NULL) return 0;
    if (getPieceColor(piece) != playerNum) return 0;

    MOVELIST *moves = possibleMoves(piece->pos, board, logMoveList, playerNum);
    if (moves == NULL) return 0;

    for (MENTRY *curr = moves->first; curr != NULL; curr = curr->next) {
        MOVE *m = curr->move;
        if (m != NULL &&
            m->pos1.x == playerMove->pos1.x &&
            m->pos1.y == playerMove->pos1.y &&
            m->pos2.x == playerMove->pos2.x &&
            m->pos2.y == playerMove->pos2.y) {
            deleteMoveList(moves);
            return 1;
        }
    }

    deleteMoveList(moves);
    return 0;
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

// move generators
MOVELIST *antMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *rookMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *knightMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *bishopMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *queenMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *kingMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);
MOVELIST *anteaterMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum);

// helpers
void addMove(MOVELIST *list, int x1, int y1, int x2, int y2);
int addMoveIfValid(MOVELIST *list,
                   PIECE *board[RANKS][FILES],
                   int x1, int y1,
                   int x2, int y2,
                   int playerNum);

MOVELIST *possibleMoves(POS currentPos, PIECE *board[RANKS][FILES], MOVELIST *logMoves, int playerNum) {
    int x = currentPos.x;
    int y = currentPos.y;

    if (x < 0 || x >= RANKS || y < 0 || y >= FILES)
        return createMoveList();

    PIECE *piece = board[x][y];
    if (piece == NULL)
        return createMoveList();

    if (getPieceColor(piece) != playerNum)
        return createMoveList();

    switch (getPieceType(piece)) {
        case Ant:
            return antMoves(currentPos, board, playerNum);

        case Rook:
            return rookMoves(currentPos, board, playerNum);
        
        case Knight:
            return knightMoves(currentPos, board, playerNum);

        case Bishop:
            return bishopMoves(currentPos, board, playerNum);

        case Queen:
            return queenMoves(currentPos, board, playerNum);

        case King:
            return kingMoves(currentPos, board, playerNum);

        case Anteater:
            return anteaterMoves(currentPos, board, playerNum);

        default:
            return createMoveList();
    }
}

MOVELIST *antMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int dir = (playerNum == 0) ? 1 : -1;
    int startRow = (playerNum == 0) ? 1 : 6;
    int newX = x + dir;

    if (newX < 0 || newX >= RANKS) return list;

    // one-square forward
    if (board[newX][y] == NULL) {
        addMove(list, x, y, newX, y);

        // two-square forward from starting row
        if (x == startRow) {
            int jumpX = x + 2 * dir;
            if (jumpX >= 0 && jumpX < RANKS && board[jumpX][y] == NULL) {
                addMove(list, x, y, jumpX, y);
            }
        }
    }

    // capture left
    if (y - 1 >= 0 && board[newX][y - 1] != NULL &&
        getPieceColor(board[newX][y - 1]) != playerNum) {
        addMove(list, x, y, newX, y - 1);
    }

    // capture right
    if (y + 1 < FILES && board[newX][y + 1] != NULL &&
        getPieceColor(board[newX][y + 1]) != playerNum) {
        addMove(list, x, y, newX, y + 1);
    }

    return list;
}

MOVELIST *rookMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    int directions[4][2] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    };

    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        int newX = x + dx;
        int newY = y + dy;

        while (addMoveIfValid(list, board, x, y, newX, newY, playerNum)) {
            newX += dx;
            newY += dy;
        }
    }

    return list;
}

MOVELIST *knightMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int jumps[8][2] = {
        { 2,  1}, { 2, -1},
        {-2,  1}, {-2, -1},
        { 1,  2}, { 1, -2},
        {-1,  2}, {-1, -2}
    };

    for (int i = 0; i < 8; i++) {
        int newX = x + jumps[i][0];
        int newY = y + jumps[i][1];

        addMoveIfValid(list, board, x, y, newX, newY, playerNum);
    }

    return list;
}

MOVELIST *bishopMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int directions[4][2] = {
        { 1,  1},   // up right
        { 1, -1},   // up left
        {-1,  1},   // down right
        {-1, -1}    // down left
    };

    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        int newX = x + dx;
        int newY = y + dy;

        while (addMoveIfValid(list, board, x, y, newX, newY, playerNum)) {
            newX += dx;
            newY += dy;
        }
    }

    return list;
}

MOVELIST *queenMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int directions[8][2] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},  // rook directions
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}   // bishop directions
    };

    for (int d = 0; d < 8; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        int newX = x + dx;
        int newY = y + dy;

        while (addMoveIfValid(list, board, x, y, newX, newY, playerNum)) {
            newX += dx;
            newY += dy;
        }
    }

    return list;
}

MOVELIST *kingMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int directions[8][2] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    };

    for (int d = 0; d < 8; d++) {
        int newX = x + directions[d][0];
        int newY = y + directions[d][1];

        addMoveIfValid(list, board, x, y, newX, newY, playerNum);
    }

    return list;
}

MOVELIST *anteaterMoves(POS currentPos, PIECE *board[RANKS][FILES], int playerNum) {
    MOVELIST *list = createMoveList();

    int x = currentPos.x;
    int y = currentPos.y;

    PIECE *piece = board[x][y];
    if (piece == NULL) return list;

    int directions[8][2] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    };

    for (int d = 0; d < 8; d++) {
        int newX = x + directions[d][0];
        int newY = y + directions[d][1];

        if (newX < 0 || newX >= RANKS || newY < 0 || newY >= FILES) {
            continue;
        }

        PIECE *target = board[newX][newY];

        // empty square: allowed
        if (target == NULL) {
            addMove(list, x, y, newX, newY);
        }
        // enemy ant: allowed capture
        else if (getPieceColor(target) != playerNum &&
                 getPieceType(target) == Ant) {
            addMove(list, x, y, newX, newY);
        }
        // everything else: not allowed
    }

    return list;
}

int addMoveIfValid(MOVELIST *list,
                   PIECE *board[RANKS][FILES],
                   int x1, int y1,
                   int x2, int y2,
                   int playerNum) {

    // out of bounds → stop
    if (x2 < 0 || x2 >= RANKS || y2 < 0 || y2 >= FILES) {
        return 0;
    }

    PIECE *target = board[x2][y2];

    // empty square → add move, continue
    if (target == NULL) {
        addMove(list, x1, y1, x2, y2);
        return 1;
    }

    // enemy piece → capture, but stop after
    if (getPieceColor(target) != playerNum) {
        addMove(list, x1, y1, x2, y2);
        return 0;
    }

    // own piece → blocked, stop
    return 0;
}

void addMove(MOVELIST *list, int x1, int y1, int x2, int y2) {
    MOVE *m = malloc(sizeof(MOVE));
    MENTRY *e = malloc(sizeof(MENTRY));

    if (m == NULL || e == NULL) {
        free(m);
        free(e);
        return;
    }

    m->pos1.x = x1;
    m->pos1.y = y1;
    m->pos2.x = x2;
    m->pos2.y = y2;

    e->move = m;
    e->prev = list->last;
    e->next = NULL;
    e->list = list;

    if (list->last != NULL) {
        list->last->next = e;
    } else {
        list->first = e;
    }

    list->last = e;
    list->length++;
}

void deleteMoveList(MOVELIST *piecePossibleMoves){
    if (piecePossibleMoves == NULL) {
        return;
    }

    MENTRY *curr = piecePossibleMoves->first;
    while (curr != NULL) {
        MENTRY *next = curr->next;
        free(curr->move);
        free(curr);
        curr = next;
    }

    free(piecePossibleMoves);
}

int logMove(MOVE *m){
        printf("Logging move.\n");
        return 1;
}

/* gamestate.c functions */
int inCheck(void){
  return 0;
}
int staleMate(void){
 return 0;
}
int checkMate(void){
 return 0;
}
