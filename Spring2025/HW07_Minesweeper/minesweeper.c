#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_MOVES 100 // Maximum moves

// Stack arrays for storing move history
int move_rows[MAX_MOVES];
int move_cols[MAX_MOVES];
int move_top = 0; // Stack top tracker

int N;
int **mines;
char **player_view;
int **revealed;

// Check if the input is the undo command
int is_undo_command(char *input) {
    return input[0] == 'u' &&
           input[1] == 'n' &&
           input[2] == 'd' &&
           input[3] == 'o' &&
           input[4] == '\0';
}

// Convert string to integer
int string_to_int(char *str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

// Push a move onto the stack
void push(int row, int col) {
    if (move_top < MAX_MOVES) {
        move_rows[move_top] = row;
        move_cols[move_top] = col;
        move_top++;
    }
}

// Pop (undo) the last move
void pop() {
    if (move_top > 0)
        move_top--;
}

// Check if the move stack is empty
int isEmpty() {
    return move_top == 0;
}

// Count number of mines adjacent to a given cell
int count_adjacent_mines(int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i == row && j == col)
                continue;
            if (i >= 0 && i < N && j >= 0 && j < N) {
                count += mines[i][j];
            }
        }
    }
    return count;
}

// Reveal all connected cells recursively if no adjacent mines
void flood_fill(int row, int col) {
    if (row < 0 || row >= N || col < 0 || col >= N)
        return;
    if (revealed[row][col] || mines[row][col])
        return;

    int count = count_adjacent_mines(row, col);
    revealed[row][col] = 1;
    player_view[row][col] = count + '0';

    if (count == 0) {
        flood_fill(row - 1, col - 1);
        flood_fill(row - 1, col);
        flood_fill(row - 1, col + 1);
        flood_fill(row, col - 1);
        flood_fill(row, col + 1);
        flood_fill(row + 1, col - 1);
        flood_fill(row + 1, col);
        flood_fill(row + 1, col + 1);
    }
}

// Reset the board to initial state
void reset_board() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            revealed[i][j] = 0;
            player_view[i][j] = '#';
        }
    }
}

// Replay all moves from the stack
void replay_moves() {
    for (int k = 0; k < move_top; k++) {
        int row = move_rows[k];
        int col = move_cols[k];
        
        if (mines[row][col]) {
            player_view[row][col] = '*';
            revealed[row][col] = 1;
        } else {
            int count = count_adjacent_mines(row, col);
            if (count == 0) {
                flood_fill(row, col);
            } else {
                revealed[row][col] = 1;
                player_view[row][col] = count + '0';
            }
        }
    }
}

// Print the players current view of the board
void print_board() {
    printf("\n   ");
    for (int j = 0; j < N; j++) {
        printf("%2d", j);
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d ", i);
        for (int j = 0; j < N; j++) {
            printf(" %c", player_view[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Check if the player has revealed all non mine cells
int check_win() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!mines[i][j] && !revealed[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// Generate the random board and save it to map.txt
void generate_map() {
    srand(time(NULL));
    int mine_count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mines[i][j] = (rand() % 10) == 0; // 1/5 chance to a mine
            mine_count += mines[i][j];
        }
    }

    if (mine_count == 0) { // Ensure at least one mine
        int i = rand() % N;
        int j = rand() % N;
        mines[i][j] = 1;
    }

    FILE *fp = fopen("map.txt", "w");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (mines[i][j]) {
                fprintf(fp, "*");
            } else {
                fprintf(fp, ".");
            }
            if (j < N - 1) fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Save the sequence of moves to moves.txt
void save_moves() {
    FILE *fp = fopen("moves.txt", "w");
    fprintf(fp, "--- Game Moves ---\n");
    for (int i = 0; i < move_top; i++) {
        fprintf(fp, "Move %d: (Row %d, Col %d)\n", i + 1, move_rows[i], move_cols[i]);
    }
    fprintf(fp, "\nTotal Moves: %d\n", move_top);
    fclose(fp);
}

int main() {
    srand(time(NULL));
    N = rand() % 9 + 2; // Random board size between 2 and 10

    // Allocate memory for board arrays
    mines = (int **)malloc(N * sizeof(int *));
    player_view = (char **)malloc(N * sizeof(char *));
    revealed = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        mines[i] = (int *)malloc(N * sizeof(int));
        player_view[i] = (char *)malloc(N * sizeof(char));
        revealed[i] = (int *)calloc(N, sizeof(int));
        for (int j = 0; j < N; j++) {
            player_view[i][j] = '#'; // All cells hidden initially
        }
    }

    generate_map(); // Create the game map

    int game_over = 0;
    int win = 0;

    // Main game loop
    while (!game_over) {
        print_board();

        char input[10];
        printf("Enter move (row col) or 'undo': ");
        if (scanf("%9s", input) == 1) {
            if (is_undo_command(input)) {
                if (isEmpty()) {
                    printf("No moves to undo.\n");
                } else {
                    pop();
                    reset_board();
                    replay_moves();
                    printf("Undo successful.\n");
                }
            } else {
                int row = string_to_int(input);
                int col;
                if (scanf("%d", &col) != 1) {
                    printf("Invalid input. Enter row and column as integers.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (row < 0 || row >= N || col < 0 || col >= N) {
                    printf("Invalid row or column. Must be between 0 and %d.\n", N - 1);
                    while (getchar() != '\n');
                    continue;
                }
                if (revealed[row][col]) {
                    printf("Cell already revealed.\n");
                    continue;
                }
                push(row, col);
                if (mines[row][col]) {
                    player_view[row][col] = '*';
                    revealed[row][col] = 1;
                    game_over = 1;
                    win = 0;
                } else {
                    int count = count_adjacent_mines(row, col);
                    if (count == 0) {
                        flood_fill(row, col);
                    } else {
                        revealed[row][col] = 1;
                        player_view[row][col] = count + '0';
                    }
                    if (check_win()) {
                        game_over = 1;
                        win = 1;
                    }
                }
            }
        } else {
            printf("Invalid input.\n");
            while (getchar() != '\n');
        }
    }

    // Game over: print final board
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (mines[i][j]) {
                player_view[i][j] = '*';
            }
        }
    }
    print_board();
    if (win) {
        printf("Congratulations! You won!\n");
    } else {
        printf("BOOM! You hit a mine. Game Over.\n");
    }

    save_moves(); // Save moves to file

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(mines[i]);
        free(player_view[i]);
        free(revealed[i]);
    }
    free(mines);
    free(player_view);
    free(revealed);

    return 0;
}