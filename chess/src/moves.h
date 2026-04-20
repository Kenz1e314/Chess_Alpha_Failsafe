/* moves.h */
/* Description:  */
/* Author(s):  */
/* History:  
        * 4/13/2026:  

*/

#ifndef MOVES_H
#define MOVES_H

#include "chess.h"

typedef struct Move MOVE;
typedef struct Mentry MENTRY;
typedef struct MoveList MOVELIST;


struct Move{
    POS pos1;
    POS pos2;
};

struct Mentry{
    MOVELIST *list;
    MENTRY *prev;
    MENTRY *next;
    MOVE *move;
};

struct MoveList{
    MENTRY *first;
    MENTRY *last;
    int length;
};



// Functions

MOVELIST *createMoveList(void);

void deleteMoveList(MOVELIST *MoveList);

void removeEntry(MOVELIST *MoveList, MENTRY *e);


#endif
