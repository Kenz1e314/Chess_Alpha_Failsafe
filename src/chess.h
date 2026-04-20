/* chess.h */
/* Description:  */
/* Author(s):  Lauren Howe */
/* History:  
        * 4/13/2026:  Started piece.h and wrote first draft
        * 4/14/2026:  Added RANKS, FILES, and SLEN definitions

*/


#ifndef PIECE_H
#define PIECE_H

#define RANKS 8
#define FILES 10
#define SLEN 40
/*
extern const int RANKS = 8;
extern const int FILES = 10;
extern const int SLEN = 40;
*/

struct Pos{
    int x; 
    int y;
};
typedef struct Pos POS;



enum PieceColor{
    White,
    Black
};
typedef enum PieceColor PIECECOLOR;

enum PieceType{
    King,
    Queen,
    Anteater,
    Bishop,
    Rook,
    Knight,
    Ant
};
typedef enum PieceType PIECETYPE;

struct Piece{
    int PieceColor;
    int PieceType;
    POS pos;
};
typedef struct Piece PIECE;


// Function declarations

/* POS getPos(PIECE *piece):  Returns position of piece */
POS getPos(PIECE *piece);

/* int getPieceType(PIECE *piece):  Returns piece type (e.g. King, Queen, etc.) */
int getPieceType(PIECE *piece);

/* int getPieceColor(PIECE *piece):  Returns piece color (White or Black) */
int getPieceColor(PIECE *piece);

/* PIECE *setPiece(PIECE *piece, PIECETYPE pieceType, PIECECOLOR pieceColor, int x, int y):  Sets values of a piece  */
PIECE *setPiece(PIECE *piece, PIECETYPE pieceType, PIECECOLOR pieceColor, int x, int y);

/* PIECE *setPos(PIECE *piece, int x, int y):  Sets position of a piece*/
PIECE *setPos(PIECE *piece, int x, int y);

#endif
