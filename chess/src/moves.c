/* moves.c */
/* Description:  */
/* Author(s):  */
/* History:  
        * 4/13/2026:  

*/

// Module/Library imports
#include "moves.h"
// Others included in moves.h

#include <stdlib.h>
#include <assert.h>


MOVELIST *createMoveList(void){
        // Create movelist structure
	MOVELIST *newMoveList;
	newMoveList = malloc(sizeof(MOVELIST));
	assert(newMoveList != NULL);

	// Initializing newMoveList
	newMoveList->length = 0;
	newMoveList->first = NULL;
	newMoveList->last = NULL;

	// return the new MoveList (ptr)
	return newMoveList;
}

#ifdef IGNORE
void deleteMoveList(MOVELIST *MoveList){
        // Check list exists
	assert(MoveList != NULL);

	// Var declaration
	MENTRY *e; // used for tracking entries
	MENTRY *nextEntry; // temp var used for e

	// Delete entries
	e = MoveList->first; // should be NULL if MoveList is empty
	while(e != NULL){
		nextEntry = e->next;
		// Deleting e (MOVE)

                // DeleteMove(e->MOVE);
		e->list = NULL;
		e->next = NULL;
		e->prev = NULL;
		free(e); // delete entry

		// Setting up for next iteration of loop
		e = nextEntry; // on last iteration, this (e) should point to NULL
	}

	// Delete list
	free(MoveList); // freeing pointer to list
	MoveList = NULL;


}
#endif
