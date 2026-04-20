/* main.c */

#include <stdio.h>
#include <stdlib.h>


#include "user.h"
#include "chess.h"
//#include "logging.h"
#include "chess.h"
#include "gamestate.h"
#include "computer.h"

void Menu(){              //check user.c
  printf("Welcome to Anteater Chess!\n");
  printf("------------------------------------\n");
  printf("Please select which game mode to play:\n");
  printf("1. Computer vs. Player\n");
  printf("2. Player vs. Player (coming soon)\n");
  printf("3.Exit\n");
}


// 40 pieces total on the board
static PIECE allPieces[40];
static int nextPieceIndex = 0;


// lists keeping track of moves made by player 1 and player 2
static MOVELIST *playerOneMoves; // essentially white's moves
static MOVELIST *playerTwoMoves; // essentially black's moves


// placing pieces on the board
PIECE *placePiece(PIECE *board[RANKS][FILES],
                  PIECETYPE type,
                  PIECECOLOR color,
                  int x, int y)
{
    // creating the piece then setting its type, color and position
    // nextPieceIndex tracks how many pieces have been created and grabs the next piece
    PIECE *p = &allPieces[nextPieceIndex++];

    // setting the pieces, type, color, positioning on the board
    setPiece(p, type, color, x, y);
    // actually placing the piece upon the board
    board[x][y] = p;
    return p;
}

void Board(PIECE *board[RANKS][FILES])
{
    nextPieceIndex = 0;


    //setting all the pieces on the board to NULL
    for (int i = 0; i < RANKS; i++)
        for (int j = 0; j < FILES; j++)
            board[i][j] = NULL;


    PIECETYPE whiteBack[FILES] = {
        Rook, Knight, Bishop, Anteater, Queen, King,
        Anteater, Bishop, Knight, Rook
    };


    PIECETYPE blackBack[FILES] = {
        Rook, Knight, Bishop, Anteater, Queen, King,
        Anteater, Bishop, Knight, Rook
    };


    for (int j = 0; j < FILES; j++)
        placePiece(board, whiteBack[j], White, 0, j);


    for (int j = 0; j < FILES; j++)
        placePiece(board, Ant, White, 1, j);


    for (int j = 0; j < FILES; j++)
        placePiece(board, blackBack[j], Black, 7, j);


    for (int j = 0; j < FILES; j++)
        placePiece(board, Ant, Black, 6, j);

    // setting other pieces to NULL
  for(int r=2; r<RANKS-2; r++){ // empty rows
    for(int f=0; f<FILES; f++){
      board[r][f] = NULL;
    }
  }
  
}


// needs to reset the gameboard, movelist, reset player to 0 (White)
void resetGame(PIECE *board[RANKS][FILES], int *playerNum){
    // reached the end of the game or exiting out prematurely
    printf("Exiting the program.\n");
    // resetting the player to 0 - White
    if (playerNum != NULL){
        *playerNum = 0;
    }
    
    // resetting also the nextPieceIndex for later creating pieces
    nextPieceIndex = 0;

    #ifdef IGNORE
    // removing entries of the moves made by the players
    // using removeLastEntry
    while (playerOneMoves -> count > 0){
        //entry = playerOneMoves -> size - 1; // index of the last entry
        removeLastEntry(playerOneMoves, entry);
    }
    while(playerTwoMoves -> count > 0){
        //entry = playerTwoMoves -> size - 1; // index of the last entry
        removeLastEntry(playerTwoMoves, entry);
    }
    #endif
    
    // just calling board again in case to reset the board state back to the initial state
    Board(board);

    // if need to clear out allPieces
    /*
    for (int i = 0; i < 40; i++)
    {
        allPieces[i] = NULL;
    }
    */

}


int switchTurn(int playerNum){
    // switching white to black
    if (playerNum == 0){
        playerNum = 1;
    }
    else {
        // black to white
        playerNum = 0;
    }
    return playerNum;
}



int main(void){
    int option = 0;
    int count = 0;
    const int PVCOMP = 1;
    const int PVP = 2;
    const int EXIT = 3;

    // Main menu prompt
    do{
        if(count > 0){
            printf("Invalid input.  Please try again.\n");
        }
      Menu();
      scanf(" %d", &option);
    } while(option != 1 && option != 2 && option != 3);

    if(option == 3){
        exit(0);
    }
    // Game
    while(1){
        
      
    }

  
  }
