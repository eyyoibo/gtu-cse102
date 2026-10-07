#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define TOTAL_SHIPS 4

char hiddenBoard[BOARD_SIZE][BOARD_SIZE];     // This board holds the ships (hidden from player)
char playerBoard[BOARD_SIZE][BOARD_SIZE];     // This board is shown to the player (with hits and misses)
int totalShotsFired = 0;                      // Counts how many times the player has fired

int shipLengths[TOTAL_SHIPS] = {4, 3, 3, 2};   // Length of each ship
int shipHitCount[TOTAL_SHIPS] = {0, 0, 0, 0};  // How many times each ship has been hit
int shipSunkStatus[TOTAL_SHIPS] = {0, 0, 0, 0};// 1 if sunk, 0 if not

int shipStartRow[TOTAL_SHIPS];               // Starting row of each ship
int shipStartColumn[TOTAL_SHIPS];            // Starting column of each ship
int shipIsHorizontal[TOTAL_SHIPS];           // 1 = horizontal, 0 = vertical
int shipMap[BOARD_SIZE][BOARD_SIZE];         // Which ship occupies each cell

// Initialize both boards with '-' character
void initializeBoards() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            hiddenBoard[row][col] = '-';
            playerBoard[row][col] = '-';
            shipMap[row][col] = -1; // no ship assigned yet
        }
    }
}

// Check if a ship of given length can be placed at position (x, y)
int canPlaceShip(int startRow, int startCol, int length, int rowIncrement, int columnIncrement) {
    for (int i = 0; i < length; i++) {
        int currentRow = startRow + i * rowIncrement;
        int currentCol = startCol + i * columnIncrement;

        if (currentRow < 0 || currentRow >= BOARD_SIZE || currentCol < 0 || currentCol >= BOARD_SIZE)
            return 0; // Out of bounds

        if (hiddenBoard[currentRow][currentCol] == 'S')
            return 0; // Overlaps another ship
    }
    return 1;
}

// Randomly place one ship on the hidden board
void placeOneShip(int shipIndex, int length) {
    int placed = 0;
    while (!placed) {
        int randomRow = rand() % BOARD_SIZE;
        int randomCol = rand() % BOARD_SIZE;
        int isHorizontal = rand() % 2; // 1 = horizontal, 0 = vertical

        int rowIncrement;
        int columnIncrement;

        if (isHorizontal == 1) {
            rowIncrement = 0;
            columnIncrement = 1;
        } else {
            rowIncrement = 1;
            columnIncrement = 0;
        }

        if (canPlaceShip(randomRow, randomCol, length, rowIncrement, columnIncrement)) {
            for (int i = 0; i < length; i++) {
                int currentRow = randomRow + i * rowIncrement;
                int currentCol = randomCol + i * columnIncrement;
                hiddenBoard[currentRow][currentCol] = 'S';
                shipMap[currentRow][currentCol] = shipIndex;
            }
            shipStartRow[shipIndex] = randomRow;
            shipStartColumn[shipIndex] = randomCol;
            shipIsHorizontal[shipIndex] = isHorizontal;
            placed = 1;
        }
    }
}

// Place all ships on the hidden board
void placeAllShips() {
    for (int i = 0; i < TOTAL_SHIPS; i++) {
        placeOneShip(i, shipLengths[i]);
    }

    FILE *file = fopen("ships.txt", "w");
    if (file != NULL) fclose(file); // clear file at start
}

// Display the player's board
void displayBoard() {
    printf("   ");
    for (int col = 0; col < BOARD_SIZE; col++) printf("%d ", col);
    printf("\n");

    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("%2d ", row);
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf("%c ", playerBoard[row][col]);
        }
        printf("\n");
    }
}

// Check if all ships are sunk
int allShipsAreSunk() {
    for (int i = 0; i < TOTAL_SHIPS; i++) {
        if (!shipSunkStatus[i]) return 0;
    }
    return 1;
}

// When a ship is sunk, write all of its cells to ships.txt (not strictly needed anymore)
void handleSunkShips() {
    for (int i = 0; i < TOTAL_SHIPS; i++) {
        if (!shipSunkStatus[i] && shipHitCount[i] == shipLengths[i]) {
            printf("Congratulations! You sank a %d-cell ship!\n", shipLengths[i]);
            shipSunkStatus[i] = 1;
        }
    }
}

// Game loop to control the gameplay
void playGame() {
    char userInput[10];
    int row, col;
    totalShotsFired = 0;

    FILE *logFile = fopen("battleship_log.txt", "a");
    if (!logFile) return; // silently skip logging if file can't be opened

    while (1) {
        displayBoard();
        printf("Enter coordinates (X to quit): ");
        fgets(userInput, sizeof(userInput), stdin);

        if (userInput[0] == 'X' || userInput[0] == 'x') {
            printf("Exiting game...\n");
            break;
        }

        if (sscanf(userInput, "%d %d", &row, &col) != 2 || row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            printf("Invalid coordinates. Please write between 0 - 9\n");
            continue;
        }

        if (playerBoard[row][col] != '-') {
            printf("This cell has already been targeted.\n");
            continue;
        }

        totalShotsFired++;

        if (hiddenBoard[row][col] == 'S') {
            playerBoard[row][col] = 'X';
            fprintf(logFile, "Shot: %d %d - HIT\n", row, col);
            printf("HIT!\n");

            int hitShipIndex = shipMap[row][col];
            shipHitCount[hitShipIndex]++;

            FILE *hitFile = fopen("ships.txt", "a");
            if (hitFile != NULL) {
                fprintf(hitFile, "%d %d %d\n", row, col, shipLengths[hitShipIndex]);
                fclose(hitFile);
            }

            handleSunkShips();
        } else {
            playerBoard[row][col] = 'O';
            fprintf(logFile, "Shot: %d %d - MISS\n", row, col);
            printf("MISS!\n");
        }

        if (allShipsAreSunk()) {
            displayBoard();
            printf("All ships are sunk! Total shots: %d\n", totalShotsFired);
            fprintf(logFile, "All ships are sunk in %d shots.\n", totalShotsFired);
            break;
        }
    }

    fclose(logFile);
}
int main() {
    srand(time(NULL)); // Seed random number generator

    while (1) {
        printf("Welcome to the Battleship Game!\n");
        initializeBoards();

        for (int i = 0; i < TOTAL_SHIPS; i++) {
            shipHitCount[i] = 0;
            shipSunkStatus[i] = 0;
        }

        placeAllShips();
        playGame();

        char restart[5];
        printf("Press 'N' to play again or 'X' to exit: ");
        fgets(restart, sizeof(restart), stdin);
        if (restart[0] == 'X' || restart[0] == 'x') {
            printf("Game terminated.\n");
            break;
        }
    }
    return 0;
}

