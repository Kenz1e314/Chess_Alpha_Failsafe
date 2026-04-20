/* main.c */

#include <stdio.h>
#include <stdlib.h>


#include "user.h"
#include "chess.h"
//#include "logging.h"
#include "gamestate.h"
#include "computer.h"
#include "moves.h"

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

void applyMove(PIECE *piece[RANKS][FILES], MOVE m) {
    PIECE *piece[m.pos1.x][m.pos1.y];
    if(piece == NULL) {
        return;
    }
    board[m.pos2.x][m.pos2.y] = piece;
    board[m.pos1.x][m.pos1.y] = NULL;
    setPos(piece, m.pos2.x, m.pos2.y);
}

int main(void){
    int option = 0;
    PIECE *board[RANKS][FILES];
    int playerNum = White;

    while (1) {
        do {
            Menu();
            scanf("%d%*c", &option);
            if (option != 1 && option != 3) {
                printf("\nInvalid option, please choose again");
            }
        } while(option != 1 && option != 3);
        if (option == 3) {
            printf("Exiting...");
            return 0;
        }
        

        // option 1
        Board(board);
        playerNum = White;
        while(1) {
            printBoard(board);

            if (playerNum == White) {
                MOVE userMove;

                printf("Enter move as: x1 y1 x2 y2: \n");
                scanf("%d %d %d %d", &userMove.pos1.x, &userMove.pos1.y, &userMove.pos2.x, &userMove.pos2.y);
                applyMove(board, userMove);
            } else {
                MOVE compMove = computerMove(board, playerNum);
                if (compMove.pos1.x == -1 || compMove.pos1.y == -1 || compMove.pos2.x == -1 || compMove.pos2.y == -1) {
                    printf("Computer has no more moves, ending game\n");
                    break;
                }
                applyMove(board, compMove);
            }
            playerNum = switchTurn(playerNum);
        }
    }
    return 0;
}
    
