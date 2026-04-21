/* computer.c */
/* Description: Provides a move of where the computer wants to go based off the 
                current state of the board and calculating the best move */
/* Author(s):  Ava Chinn */
/* Editor(s): Alonso De La Cruz*/
/*Decription: Added EndGame function for king and implemented it in getPieceSquareValue 
              by adding if statement, and removed extra row for KingMidTable */
/* History:  
        * 4/16/2026:  1st draft -Ava
        *4/16/2026:   2nd draft -Alonso
        *4/17/2026:   Updated EndGame criteria/logic -Ava
        *4/18.2026:  Changed computerMove to return a move, added a freeBoard function -Ava

*/
#include <stdio.h>
#include <stdlib.h>
#include "computer.h"
#include "moves.h"
#include "stubfunctions.h"




// Returns the value of each piece in centipawns 
static int getPieceBaseValue(PIECE *piece) {
        switch(getPieceType(piece)) {
                case Ant: 
                        return 100;
                case Anteater:
                        return 300;
                case Knight: 
                        return 320;
                case Bishop: 
                        return 330;
                case Rook: 
                        return 500;
                case Queen:
                        return 900;
                case King:
                        return 20000;
                default:
                        return 0;
        }
}

static const int antTable[8][10] = {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},
        { 10, 10, 10, 20, 30, 30, 20, 10, 10, 10},
        {  5,  5, 10, 10, 25, 25, 10, 10,  5,  5},
        {  0,  0,  0,  0, 20, 20,  0,  0,  0,  0},
        {  5,  5, -5,-10,  0,  0,-10, -5,  5,  5},
        {  5,  5, 10, 10,-20,-20, 10, 10,  5,  5},
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
};

static const int anteaterTable[8][10] = {
        {-20,-20,-20,-20,-20,-20,-20,-20,-20,-20},
        {  0,  0,  0,  0,  5,  5,  0,  0,  0,  0},
        {  5, 10, 20, 20, 30, 30, 20, 20, 10,  5},
        {  0,  5, 10, 20, 20, 20, 20, 10,  5,  0},
        {  0,  5, 10, 20, 20, 20, 20, 10,  5,  0},
        {  0,  0,  5, 10, 10, 10, 10,  5,  0,  0},
        {  0,  0,  0,  0,  5,  5,  0,  0,  0,  0},
        {-20,-20,-10,-10,-10,-10,-10,-10,-20,-20},
};

static const int knightTable[8][10] = {
        {-50,-40,-40,-30,-30,-30,-30,-40,-40,-50},
        {-40,-20,  0,  0,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 20, 20, 15,  0,-30},
        {-30,  5, 15, 20, 20, 20, 20, 15,  5,-30},
        {-40, 20,  0,  5,  5,  5,  5,  0,-20,-40},
        {-50,-40,-40,-30,-30,-30,-30,-40,-40,-50}
};

static const int bishopTable[8][10] = {
        {-20,-10,-10,-10,-10,-10,-10,-10,-10,-20},
        {-10,-10,  0,  0,  0,  0,  0,  0,-10,-10},
        {-10,  0,  0,  5, 10, 10,  5,  0,  0,-10},
        {-10,  5,  5,  5, 10, 10,  5,  5,  5,-10},
        {-10,  0,  0, 10, 10, 10, 10,  0,  0,-10},
        {-10,  0, 10, 10, 10, 10, 10, 10,  0,-10},
        {-10,  5,  0,  0,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-10,-10,-20}
};

static const int rookTable[8][10] = {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {  5, 10, 10, 10, 10, 10, 10, 10, 10,  5},
        { -5  ,0,  0,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0,  0,  0, -5},
        {  0,  0,  0,  0,  5,  5,  0,  0,  0,  0}
};

static const int queenTable[8][10] = {
        {-20,-10,-10, -5, -5, -5, -5,-10,-10,-20},
        {-10, -5,  0,  0,  0,  0,  0,  0, -5,-10},
        {-10,  0,  5,  5,  5,  5,  5,  5,  0,-10},
        { -5,  0,  5,  5,  5,  5,  5,  5,  0, -5},
        {  0,  0,  5,  5,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5, -5, -5,-10,-10,-20}
};

static const int kingMidTable[8][10] = {
        {-30,-40,-40,-40,-50,-50,-40,-40,-40,-30},
        {-30,-40,-40,-40,-50,-50,-40,-40,-40,-30},
        {-30,-40,-40,-40,-50,-50,-40,-40,-40,-30},
        {-30,-40,-40,-40,-50,-50,-40,-40,-40,-30},
        {-20,-30,-30,-30,-40,-40,-30,-30,-30,-20},
        {-10,-20,-20,-20,-20,-20,-20,-20,-20,-10},
        { 20, 20,  0,  0,  0,  0,  0,  0, 20, 20},
        { 20, 30, 10,  0,  0,  0,  0, 10, 30, 20}
};

static const int kingEndTable[8][10] = {          
        {-50,-40,-30,-20,-20,-20,-20,-30,-40,-50},
        {-30,-20,-10,  0,  0,  0,  0,-10,-20,-30},
        {-30,-10, 20, 20, 30, 30, 20, 20,-10,-30},
        {-30,-10, 30, 40, 40, 40, 40, 30,-10,-30},
        {-30,-10, 30, 40, 40, 40, 40, 30,-10,-30},
        {-30,-10, 20, 30, 30, 30, 30, 20,-10,-30},
        {-30,-30,  0,  0,  0,  0,  0,  0,-30,-30},
        {-50,-30,-30,-30,-30,-30,-30,-30,-30,-50}
};

/* Adjusts table orientation.
   For White's perspective, use [y][x], else use [RANKS - 1 - y][x] */
static int tableLookup(const int table[8][10], PIECE *piece) {
        POS p = getPos(piece);
        int x = p.x;
        int y = p.y;

        if (getPieceColor(piece) == White) {
                return table[y][x];
        } else {
                return table[RANKS - 1 - y][x];
        }
}




/* EndGame, a function that would decide when the king would move and when
the endgame starts, here "endgame starts when there are no queens or until there is a certain 
amount of pieces left. the amount of pieces is decided base on the points left on the board*/
static int EndGame(PIECE *board[RANKS][FILES]){
    int x, y;
    int wTotalPoints = 0;
    int bTotalPoints = 0;
    int queens = 0;
    for(y = RANKS - 1; y >= 0; y--){
        for(x = 0; x < FILES; x++){
            PIECE *piece = board[y][x];
            if (piece == NULL){
                continue;
            }
            
            if (getPieceType(piece) == Queen){
                queens++;
            }

            if (getPieceColor(piece) == White) {
                wTotalPoints += getPieceBaseValue(piece);
            }
            else if (getPieceColor(piece) == Black) {
                bTotalPoints += getPieceBaseValue(piece);
            }
        }
    }
    // if neither side has a queen
    if (queens == 0) {
        return 1;
    }
    // if each side has a queen and no more than 1 minor piece
    else if (queens == 2) {
        if (wTotalPoints <= 21400 && bTotalPoints <= 21400) {
                return 1;
        }
    }
    return 0;  
}

/* Helper function for tableLoopup, returns position's bonus or penalty */
static int getPieceSquareValue(PIECE *piece, PIECE *board[RANKS][FILES]) {
        switch(getPieceType(piece)) {
                case Ant:
                        return tableLookup(antTable, piece);
                case Anteater:
                        return tableLookup(anteaterTable, piece);
                case Knight:
                        return tableLookup(knightTable, piece);
                case Bishop:
                        return tableLookup(bishopTable, piece);
                case Rook:
                        return tableLookup(rookTable, piece);
                case Queen:
                        return tableLookup(queenTable, piece);
                case King:
                        if(EndGame(board)){
                            return tableLookup(kingEndTable, piece);
                        }
                        else{
                            return tableLookup(kingMidTable, piece);
                        }
        }
        return 0;
}


/* Scores a single piece: base value + position */
static int evalPiece( PIECE *board[RANKS][FILES], PIECE *piece) {
        if (piece == NULL) {
                return 0;
        }
        return getPieceBaseValue(piece) + getPieceSquareValue(piece,board);
}

/* Scores board position, +score for computer piece, -score for opponent's pieces */
/* Final score = how good of a position */
static int evalBoard(PIECE *board[RANKS][FILES], int playerNum) {
        int x, y;
        int score = 0;

        for (y = RANKS - 1; y >= 0; y--) {
                for (x = 0; x < FILES; x++) {
                        PIECE *piece = board[y][x];
                        if (piece == NULL) {
                                continue;
                        }

                        int value = evalPiece(board,piece);

                        if (getPieceColor(piece) == playerNum) {
                                score += value;
                        } else {
                                score -= value;
                        }
                }
        }
        return score;
}

/* Creates copy of board for simulated moves */
static void copyBoard(PIECE *src[RANKS][FILES], PIECE *dest[RANKS][FILES]) {
        int x, y;

        for (y = RANKS - 1; y >= 0; y--) {
                for (x = 0; x < FILES; x++) {
                        if (src[y][x] == NULL) {
                                dest[y][x] = NULL;
                        } else {
                                dest[y][x] = malloc(sizeof(PIECE));
                                if (dest[y][x] != NULL) {
                                        *dest[y][x] = *src[y][x];
                                }
                        }
                }
        }
}

/* Frees the copy of board */
static void freeBoard(PIECE *board[RANKS][FILES]) {
        int x, y;
        for (y = RANKS - 1; y >= 0; y--) {
                for (x = 0; x < FILES; x++) {
                        if(board[y][x] != NULL) {
                                free(board[y][x]);
                                board[y][x] = NULL;
                        }
                }
        }
}

/* Simulates moves on the board copy */
static void moveOnTempBoard(MOVE *move, PIECE *board[RANKS][FILES]) {
        PIECE *piece = board[move->pos1.y][move->pos1.x];

        if (piece == NULL) {
                return;
        }
        if (piece != NULL) {
                free(board[move->pos2.x][move->pos2.y]);
        }

        board[move->pos2.y][move->pos2.x] = piece;
        board[move->pos1.y][move->pos1.x] = NULL;
        setPos(piece, move->pos2.x, move->pos2.y);
}

// /* Creates new move and copies another move into it */
// static MOVE *copyMove(MOVE *m) {
//         MOVE *newMove = malloc(sizeof(MOVE));
//         if (newMove == NULL) {
//                 return NULL;
//         }
//         *newMove = *m;
//         return newMove;
// }


/* Returns move calculated by computer */ 
MOVE computerMove(PIECE *board[RANKS][FILES], int playerNum) {
        int x, y;
        int bestscore = -1000000000;
        
        MOVE bestMove;
        bestMove.pos1.x = -1;
        bestMove.pos1.y = -1;
        bestMove.pos2.x = -1;
        bestMove.pos2.y = -1;
  
        for (y = RANKS - 1; y >= 0; y--) {
                for (x = 0; x < FILES; x++) {
                        PIECE *piece = board[y][x];
                        if (piece == NULL) {
                                continue;
                        }

                        if (getPieceColor(piece) != playerNum) {
                                continue;
                        }

                        POS start = getPos(piece);
                        MOVELIST *moves = possibleMoves(start, board, NULL, playerNum);
                        if (moves == NULL) {
                                continue;
                        }

                        for (MENTRY *current = moves->first; current != NULL; current = current->next) {
                                MOVE *m = current->move;
                                if (m == NULL) {
                                        continue;
                                }

                                
                                PIECE *tempBoard[RANKS][FILES];

                                copyBoard(board, tempBoard);
                                moveOnTempBoard(m, tempBoard);

                                int score = evalBoard(tempBoard, playerNum);
                                freeBoard(tempBoard);

                                if (score > bestscore) {
                                        bestscore = score;
                                        bestMove = *m;
                                } 
                        }
                }
        }
        return bestMove;
}
