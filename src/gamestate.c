/* gameState.c
/* Description:  */
/* Author(s):  Sanaa Bebal & Lauren Howe
   Modifications:
   04/15/2026 LH Created file and worked on makeMove()
   04/16/2026 SB initial modifications
              LH Finished first draft of simple version of makeMove(), 
                        inCheck(), and staleMate()
   04/17/2026 LH Finished first draft of castling and en passant and makeMove()
                        Fixes:
                                * removed final else for makeMove() (Updating board called by en passant segment)
                                * in inCheck(), added condition to make sure that checkPiece ptr is not NULL
                                * fixed a call of legalMove() to have correct params
                                * initialized piecePossibleMoves var to NULL
   04/18/2026 SB iniital logic for checkMate
              LH Made more modifications
                        Fixes:
                                * made testing code cleaner and more effective
                                * added logMove() in makeMove()
                                * fixed some function calls
                                * fixed castling code (still had some code from when I misunderstood its behavior)
                                * made some small fixes between V1.2 and V1.3 regarding en passant and other things
                                * made changes that reflect that playerNum is 0 or 1, not 1 or 2
   04/19/2026 SB added additional helper functions and updated logic for checkMate
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chess.h"
#include "moves.h"
#include "gamestate.h"
#include "user.h"


void makeMove(int playerNum, int isComputer, PIECE *board[RANKS][FILES], MOVELIST *logMoveList){
        char inputStr[SLEN];
        MOVE playerMove;
        int count = 0;
        int playerNum12 = playerNum + 1; // player num as 1 or 2

        // internal move trackers
        //static int player1Castled = 0; // castling can only be done once
        //static int player2Castled = 0; // castling can only be done once

        // Prompt user for move (if applicable) and register move
        if(!isComputer){
                do{
                        if(count > 0){
                                if(strcmp(inputStr, "help") == 0){
                                        helpMenu();
                                } else if(strcmp(inputStr, "quit") == 0){
                                        printf("Exiting program.\n");
                                        exit(0);
                                }
                                printf("Not a move, or move is illegal.  Please try again.\n");
                        }
                        printf("Player %d's turn:  ", playerNum12);
                        scanf(" %15[^\n]", inputStr); // scans until newline--%s only handles strings w/o spaces
                        playerMove = parseMove(inputStr);

                        count++;
                } while(playerMove.pos1.x == -1 || !legalMove(&playerMove, board, playerNum, logMoveList));
        } else{
                //#ifndef TESTING
                playerMove = computerMove(board, playerNum);
                //#endif
        }
        
        #ifdef TESTING
        printf("Move is registered.\n");
        #endif

        // Processing the move
        int rank1 = playerMove.pos1.x;
        int file1 = playerMove.pos1.y;
        int rank2 = playerMove.pos2.x;
        int file2 = playerMove.pos2.y;
        PIECE *primaryPiece;
        PIECE *secondaryPiece;

        // Checking that it is not a move w/ unique response (ex:  castling, pawn promotion, anteating, etc.)
        

        //#ifndef TESTING

        /* Castling:  Moves king two spaces closer to rook and moves rook to adjacent square king just crossed */
        // Note:  castling handles the king's move as input
        if(castling(&playerMove, board, logMoveList)){ // checks if move is valid castling move and that it hasn't been called successfully by that player before
                // Primary is king; secondary is rook
                primaryPiece = board[rank1][file1];
                if(file1 > file2){ // king moves left--rook is to the left
                        secondaryPiece = board[rank1][0];
                } else { // king moves right -- rook is to the right
                        secondaryPiece = board[rank1][FILES-1];
                }
                
                #ifdef TESTING
                        printf("Executing castling.  Primary piece %p, secondary piece %p\n", primaryPiece, secondaryPiece);
                        printf("King location:  rank %d file %d\n", primaryPiece->pos.x, primaryPiece->pos.y);
                #endif
                // Moving the king two spaces closer to rook (direction depends on rook's position left/right of king)
                /*if(primaryPiece->pos.y - secondaryPiece->pos.y > 0 ){// king is to the right of rook--king moves left
                        primaryPiece->pos.y -= 2; // setting PIECE's internal pos
                } else{ // king is to the left of rook--king moves right
                        primaryPiece->pos.y += 2; // setting PIECE's internal pos
                }*/
                primaryPiece->pos.y = file2; // setting king's internal pos
                board[rank2][file2] = primaryPiece; // moving king
                board[rank1][file1] = NULL; // setting old king square to empty

                #ifdef TESTING
                        printf("King has moved.  New position:  rank %d file %d\n", primaryPiece->pos.x, primaryPiece->pos.y);
                #endif

                // Moving rook (depends on if king is to the left or to the right of rook)
                #ifdef TESTING
                        printf("Rook position:  rank %d file %d\n", secondaryPiece->pos.x, secondaryPiece->pos.y);
                #endif
                int rookOldRank = secondaryPiece->pos.x;
                int rookOldFile = secondaryPiece->pos.y;
                int rookNewFile = -1;
                if(primaryPiece->pos.y - secondaryPiece->pos.y > 0 ){ // rook is to the left of king--rook moves to one square right of king
                        rookNewFile = primaryPiece->pos.y + 1; // setting rook's internal pos one right of king
                } else{ // rook is to the right of king--rook moves to one square left of king
                        rookNewFile = primaryPiece->pos.y - 1;
                }
                // Setting rook's position
                secondaryPiece->pos.y = rookNewFile; // setting internal pos of rook
                board[rank2][rookNewFile] = secondaryPiece; // moving rook
                board[rookOldRank][rookOldFile] = NULL; // setting old king square to empty


                // Wraupup:  Show that player has now castled; log move; end turn
                printf("Player %d has castled.\n", playerNum12);
                logMove(&playerMove);

                return; 

        }

        /* En Passant:  Pawn moves diagonally and takes pawn "behind" its new position */
        // Note:  en passant just handles the attacking pawn's move as input
        if(enPassant(&playerMove, board, logMoveList)){ // checks if move is valid/legal en passant move
                primaryPiece = board[rank1][file1]; // attacking pawn

                #ifdef TESTING
                        printf("Attacking pawn pos:  rank %d, file %d\n", primaryPiece->pos.x, primaryPiece->pos.y);
                #endif
                
                // Finding "victim" pawn and removing it from board
                if(file2 > file1){ // attacking pawn moves right--captured pawn is to the right of starting position
                        secondaryPiece = board[rank1][file1+1];
                        secondaryPiece->pos.x = -1; secondaryPiece->pos.y = -1; // setting victim pawn's internal coords
                        board[rank1][file1+1] = NULL; // removing piece from board

                } else{ // attacking pawn moves left--captured pawn is to the left of starting position
                        secondaryPiece = board[rank1][file1-1];
                        secondaryPiece->pos.x = -1; secondaryPiece->pos.y = -1; // setting victim pawn's internal coords
                        board[rank1][file1-1] = NULL; // removing piece from board
                }

                // Wrapup:  No return statement--attacking pawn still needs to be moved
                printf("Player %d executed en passant.\n", playerNum12);
        }

        

        //#endif
        
        // Updating the board
        // Checking if there is another piece already there on the board (capture)
        primaryPiece = board[playerMove.pos1.x][playerMove.pos1.y]; // ZZZ:  Removed * before primaryPiece 4/17/2026--double check that this is correct?
        secondaryPiece = board[playerMove.pos2.x][playerMove.pos2.y];
        if(secondaryPiece != NULL){ // Capture
                setPos(secondaryPiece, -1, -1); // sets piece to location off the board
        }
        setPos(primaryPiece, playerMove.pos2.x, playerMove.pos2.y); // set primary piece's location
        board[playerMove.pos2.x][playerMove.pos2.y] = primaryPiece; // setting (x2, y2) on the board to primary piece
        board[playerMove.pos1.x][playerMove.pos1.y] = NULL; // setting (x1, y1) to empty

        // Wrapup:  Logging move
        logMove(&playerMove);
}
