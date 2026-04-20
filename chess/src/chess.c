/* chess.c */
/* Description:  */
/* Author(s):  Lauren Howe */
/* History:  
        * 4/13/2026:  Created piece.c and wrote first draft of all functions

*/

// Module/Library imports
#include "chess.h"

POS getPos(PIECE *piece){
    return piece->pos;
}

int getPieceType(PIECE *piece){
    return piece->PieceType;
}

int getPieceColor(PIECE *piece){
    return piece->PieceColor;
}

PIECE *setPiece(PIECE *piece, PIECETYPE pieceType, PIECECOLOR pieceColor, int x, int y){
    piece->pos.x = x;
    piece->pos.y = y;
    piece->PieceType = pieceType;
    piece->PieceColor = pieceColor;
    return piece;
}

PIECE *setPos(PIECE *piece, int x, int y){
    piece->pos.x = x;
    piece->pos.y = y;
    return piece;
}
