#include <stdio.h>

#define CUPS 6

void initializeGame(int board[2][7], int n) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < CUPS; j++) {
            board[i][j] = n;  // Set n stones in each cup
        }
        board[i][6] = 0;  // Large cups empty starts
    }
}

void printBoard(int board[2][7]) {
    printf("+----+---+---+---+---+---+---+----+\n");
    printf("|    | %d | %d | %d | %d | %d | %d |    |\n", board[0][5], board[0][4], board[0][3], board[0][2], board[0][1], board[0][0]);
    printf("|  %d |---+---+---+---+---+---| %d  |\n", board[1][6], board[0][6]);
    printf("|    | %d | %d | %d | %d | %d | %d |    |\n", board[1][0], board[1][1], board[1][2], board[1][3], board[1][4], board[1][5]);
    printf("+----+---+---+---+---+---+---+----+\n");
}

int main() {
    int board[2][CUPS + 1];
    int n = 4; // Number of stone to initialize each cups
    
    initializeGame(board, n);
    printBoard(board);
    
    
     // Understand the  who is winner and printing result
    int p1score = board[0][CUPS];
    int pcscore = board[1][CUPS];
    
    if (p1score > pcscore) {
        printf("Player 1 wins, score is %d\n", p1score);
    } 
    else if (pcscore > p1score) {
        printf("Computer wins, score is %d\n", pcscore);
    } 
    else {
        printf("It is a tie\n");
    }

    return 0;
}
