/* Chess.c : 
*  Author: Sanaa Bebal & Lauren Howe
*  Modifications: 
*  04/14/2026 SB initial modifications
   04/14/2026 LH initial modifications
   04/16/2026 SB fixed logic for printing out board
              LH Fixed parseMove() (replaced x and y w/ rank and file, respectively) to fix coordinate system
*/

/* NOTES 
    Origin is at A1

    Board is set up as 
    // Storing system 1
    {
    {A1, B1, C1, },
    {A2, B2, C2, },
    etc.
    }
*/

// #define TESTING

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "chess.h"
#include "user.h"

#ifdef TESTING
#include "moves.h"
#endif

// prints main menu (should this be in main.c itself)
void printMenu(void){
    char gameMode[16];
    char playerColor[16];

    printf("Welcome to Anteater Chess!\n");
    printf("What game mode do you wish to play (PvP, CvC or CvP): ");
    if (scanf("%15s", gameMode) != 1) {
        return;
    }

    printf("What color do you wish to be (White or Black): ");
    if (scanf("%15s", playerColor) != 1) {
        return;
    }

    printf("Have a good game! If you have any questions or want to exit, type HELP or exit.\n");
    printf("Good luck!\n");
}

// print out the current state of the board 8x10 (8 rows, 10 cols)
void printBoard(PIECE *board[RANKS][FILES]){
    // need the functionalities from chess.c
    /*
    PIECE has PieceColor, PieceType, POS
      +----+----+----+----+----+----+----+----+----+----+
    8 |    |    |    |    |    |    |    |    |    |    |
      +----+----+----+----+----+----+----+----+----+----+
      ...
      +----+----+----+----+----+----+----+----+----+----+
    1 |    |    |    |    |    |    |    |    |    |    |
      +----+----+----+----+----+----+----+----+----+----+ 
        A    B    C    D    E    F    G    H    I    J
    */


    // iterating through the 2D list to print out pieces 
    for (int i = RANKS - 1; i >= 0; i--) {
        printf("  +----+----+----+----+----+----+----+----+----+----+\n");
        printf("%d", i + 1);
        for (int j = 0; j < FILES; j++) {
            // needs to grab the piece type so it can write it out on the board
            if (board[i][j] == NULL) {
                printf("|    ");

            }
            else {
                char c = pieceTypeToChar(getPieceType(board[i][j]));
                printf("| %c  ", c);
            }
        }
        printf("|\n");
    }

    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("    A    B    C    D    E    F    G    H    I    J\n");
}



// (for the GUI will be a scroll down menu) print out menu for any FAQs
void helpMenu(void){
    printf("Help Menu:  \n");

    printf("Making a move:  Enter the the rank, then the file of the starting position, then the ending position.  Please use upper case for letters, and put one space between moves.\n");
    printf("\tExample:  \n");
    printf("\tPlayer 1's turn:  E2 E4\n");

    printf("Opening the help menu:  Type in \'help\' in all lowercase to access the help menu.\n");

    printf("Quitting the game:  Type in \'quit\' in all lowercase to quit.\n\n");
}

// creates a display indicating whether a win/loss or stalemate has occurred
void endGame(int isCheckmate, int isStalemate){
    if(isCheckmate != 0 && isStalemate != 0){
        printf("ERROR:  Cannot determine who won the game - both stalemate and checkmate triggered.\n\n");
        return;
    }
    if(isCheckmate == 1){
        printf("Player 1 is in checkmate!\n");
        printf("Player 2 wins!\n\n");
    } else if(isCheckmate == 2){
        printf("Player 2 is in checkmate!\n");
        printf("Player 1 wins!\n\n");
    }
    if(isStalemate){
        printf("No player can win.\n");
        printf("The game ends in a stalemate!\n\n");
    }
    return;
}

// provided with a user input for a move, returns back a MOVE
MOVE parseMove(char *inputStr){ // Ex inputStr:  "E2 E4"
    // Default move
    MOVE answer = {{-1, -1},{-1, -1}};

    // Default:  Invalid length (too short) --> invalid move
    if(strlen(inputStr) < 5){
        #ifdef TESTING
        printf("String is too short!\n");
        #endif
        return answer;
    }

    // Pulling rank and file characters
    char file1 = inputStr[0]; // Ex:  E
    char rank1 = inputStr[1]; // Ex:  2
    char file2 = inputStr[3];
    char rank2 = inputStr[4];

    #ifdef TESTING
    printf("Move (testing1):  (%c, %c) to (%c, %c)\n", file1, rank1, file2, rank2);
    #endif
    
    // Calculating y (file) number:  A --> 0, B --> 1, etc.
    int file1Num = file1 - 'A';
    int file2Num = file2 - 'A';
    // Calculating x (rank) number:  1 --> 0, 2 --> 1, etc.
    int rank1Num = rank1 - '1';
    int rank2Num = rank2 - '1';

    #ifdef TESTING
    printf("Move (testing2):  rank num %d, file num %d to rank num %d, file num %d\n", rank1Num, file1Num, rank2Num, file2Num);
    #endif

    // Setting values to default if invalid
    if(file1Num < 0 || file1Num >= FILES || file2Num < 0 || file2Num >= FILES || 
        rank1Num < 0 || rank1Num >= RANKS || rank2Num < 0 || rank2Num >= RANKS){
            rank1Num = rank2Num = file1Num = file2Num = -1;
    }

    #ifdef TESTING
    printf("Move:  rank num %d, file num %d to rank num %d, file num %d)\n", rank1Num, file1Num, rank2Num, file2Num);
    #endif

    answer.pos1.x = rank1Num; answer.pos1.y = file1Num;
    answer.pos2.x = rank2Num; answer.pos2.y = file2Num;

    return answer;
}

#ifdef TESTINGUSERMAIN
int main(void){
    // helpMenu();

    /* 
    endGame(0, 0); // nothing - continuing game
    endGame(0, 1); // stalemate
    endGame(1, 0); // player 1 in checkmate - player 2 wins
    endGame(1, 1); // ERROR
    endGame(2, 0); // player 2 in checkmate - player 1 wins
    */

    MOVE myMove = parseMove("");
    printf("MOVE:  (%d, %d), to (%d, %d)\n", myMove.pos1.x, myMove.pos1.y, myMove.pos2.x, myMove.pos2.y);
    myMove = parseMove("E2 E4");
    printf("MOVE:  (%d, %d), to (%d, %d)\n", myMove.pos1.x, myMove.pos1.y, myMove.pos2.x, myMove.pos2.y);
    myMove = parseMove("Hello World!");
    printf("MOVE:  (%d, %d), to (%d, %d)\n", myMove.pos1.x, myMove.pos1.y, myMove.pos2.x, myMove.pos2.y);
    myMove = parseMove("A1 J8");
    printf("MOVE:  (%d, %d), to (%d, %d)\n", myMove.pos1.x, myMove.pos1.y, myMove.pos2.x, myMove.pos2.y);
    myMove = parseMove("J1 A8");
    printf("MOVE:  (%d, %d), to (%d, %d)\n", myMove.pos1.x, myMove.pos1.y, myMove.pos2.x, myMove.pos2.y);

    return 0;
}

#endif
