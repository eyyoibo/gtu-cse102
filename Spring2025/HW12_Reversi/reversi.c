#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    CELL_EMPTY = 0,
    CELL_USER = 1,      /* 'O' or 'o' */
    CELL_COMPUTER = 2   /* 'X' or 'x' */
} CellType;

typedef struct {
    int row;
    int col;
} Coordinate;

/* Function prototypes */
CellType** create_board(int size);
void free_board(CellType** board, int size);
void init_board(CellType** board, int size);
void display_board(CellType** board, int size);
int is_valid_coord(int r, int c, int size);
int count_flips_direction(CellType** board, int size, int r, int c, int dr, int dc, CellType player);
int count_total_flips(CellType** board, int size, int r, int c, CellType player);
int is_legal_move(CellType** board, int size, int r, int c, CellType player);
int has_legal_moves(CellType** board, int size, CellType player);
void apply_move(CellType** board, int size, int r, int c, CellType player);
Coordinate get_computer_move(CellType** board, int size);
void count_scores(CellType** board, int size, int *user_score, int *comp_score);
void play_game(void);

CellType** create_board(int size) {
    int i;
    CellType** board = (CellType**)malloc(size * sizeof(CellType*));
    if (!board) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < size; i++) {
        board[i] = (CellType*)malloc(size * sizeof(CellType));
        if (!board[i]) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    return board;
}

void free_board(CellType** board, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

void init_board(CellType** board, int size) {
    int i, j;
    int mid = size / 2;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = CELL_EMPTY;
        }
    }
    /* Initial 4 pieces in center */
    board[mid - 1][mid - 1] = CELL_COMPUTER; /* X */
    board[mid - 1][mid] = CELL_USER;         /* O */
    board[mid][mid - 1] = CELL_USER;         /* O */
    board[mid][mid] = CELL_COMPUTER;         /* X */
}

void display_board(CellType** board, int size) {
    int i, j;
    printf("\n  ");
    for (j = 0; j < size; j++) {
        printf("%c", 'a' + j);
    }
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%-2d", i + 1);
        for (j = 0; j < size; j++) {
            if (board[i][j] == CELL_USER) {
                printf("o");
            } else if (board[i][j] == CELL_COMPUTER) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int is_valid_coord(int r, int c, int size) {
    return (r >= 0 && r < size && c >= 0 && c < size);
}

int count_flips_direction(CellType** board, int size, int r, int c, int dr, int dc, CellType player) {
    CellType opp = (player == CELL_USER) ? CELL_COMPUTER : CELL_USER;
    int curr_r = r + dr;
    int curr_c = c + dc;
    int opp_count = 0;

    while (is_valid_coord(curr_r, curr_c, size) && board[curr_r][curr_c] == opp) {
        opp_count++;
        curr_r += dr;
        curr_c += dc;
    }

    if (opp_count > 0 && is_valid_coord(curr_r, curr_c, size) && board[curr_r][curr_c] == player) {
        return opp_count;
    }
    return 0;
}

int count_total_flips(CellType** board, int size, int r, int c, CellType player) {
    int dr, dc;
    int total = 0;

    if (board[r][c] != CELL_EMPTY) {
        return 0;
    }

    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            total += count_flips_direction(board, size, r, c, dr, dc, player);
        }
    }
    return total;
}

int is_legal_move(CellType** board, int size, int r, int c, CellType player) {
    return (count_total_flips(board, size, r, c, player) > 0);
}

int has_legal_moves(CellType** board, int size, CellType player) {
    int r, c;
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            if (is_legal_move(board, size, r, c, player)) {
                return 1;
            }
        }
    }
    return 0;
}

void apply_move(CellType** board, int size, int r, int c, CellType player) {
    int dr, dc;
    CellType opp = (player == CELL_USER) ? CELL_COMPUTER : CELL_USER;

    board[r][c] = player;

    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            int flips;
            if (dr == 0 && dc == 0) continue;
            flips = count_flips_direction(board, size, r, c, dr, dc, player);
            if (flips > 0) {
                int curr_r = r + dr;
                int curr_c = c + dc;
                while (board[curr_r][curr_c] == opp) {
                    board[curr_r][curr_c] = player;
                    curr_r += dr;
                    curr_c += dc;
                }
            }
        }
    }
}

Coordinate get_computer_move(CellType** board, int size) {
    Coordinate best_move;
    int max_flips = -1;
    int r, c;

    best_move.row = -1;
    best_move.col = -1;

    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            int flips = count_total_flips(board, size, r, c, CELL_COMPUTER);
            if (flips > max_flips) {
                max_flips = flips;
                best_move.row = r;
                best_move.col = c;
            }
        }
    }
    return best_move;
}

void count_scores(CellType** board, int size, int *user_score, int *comp_score) {
    int r, c;
    *user_score = 0;
    *comp_score = 0;
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            if (board[r][c] == CELL_USER) {
                (*user_score)++;
            } else if (board[r][c] == CELL_COMPUTER) {
                (*comp_score)++;
            }
        }
    }
}

void play_game(void) {
    int size = 0;
    CellType** board;
    int user_score, comp_score;
    char buffer[64];

    printf("===================================\n");
    printf("         REVERSI / OTHELLO         \n");
    printf("===================================\n");

    while (1) {
        printf("Enter board size (even number between 4 and 20, e.g. 8): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return;
        }
        if (sscanf(buffer, "%d", &size) == 1 && size >= 4 && size <= 20 && size % 2 == 0) {
            break;
        }
        printf("Invalid board size! Must be an even number between 4 and 20.\n");
    }

    board = create_board(size);
    init_board(board, size);
    display_board(board, size);

    while (1) {
        int user_can_play = has_legal_moves(board, size, CELL_USER);
        int comp_can_play = has_legal_moves(board, size, CELL_COMPUTER);

        if (!user_can_play && !comp_can_play) {
            printf("No legal moves left for either player. Game over!\n");
            break;
        }

        /* User's Turn */
        if (user_can_play) {
            while (1) {
                int row = -1;
                char col_char = '\0';
                int col = -1;

                printf("Your turn (O). Enter move (e.g. 4c): ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    free_board(board, size);
                    return;
                }
                buffer[strcspn(buffer, "\r\n")] = '\0';

                if (sscanf(buffer, "%d%c", &row, &col_char) == 2) {
                    col_char = (char)tolower((unsigned char)col_char);
                    row = row - 1; /* 0-indexed */
                    col = col_char - 'a';

                    if (is_valid_coord(row, col, size) && is_legal_move(board, size, row, col, CELL_USER)) {
                        apply_move(board, size, row, col, CELL_USER);
                        printf("You played: %d%c\n", row + 1, col_char);
                        display_board(board, size);
                        break;
                    }
                }
                printf("Illegal move! Please try another position.\n");
            }
        } else {
            printf("You have no legal moves. Passing turn to computer...\n");
        }

        /* Computer's Turn */
        comp_can_play = has_legal_moves(board, size, CELL_COMPUTER);
        if (comp_can_play) {
            Coordinate comp_move = get_computer_move(board, size);
            apply_move(board, size, comp_move.row, comp_move.col, CELL_COMPUTER);
            printf("Computer (X) played: %d%c\n", comp_move.row + 1, 'a' + comp_move.col);
            display_board(board, size);
        } else {
            printf("Computer has no legal moves. Passing turn to you...\n");
        }
    }

    count_scores(board, size, &user_score, &comp_score);
    printf("===================================\n");
    printf("Final Score:\n");
    printf("User (O):     %d\n", user_score);
    printf("Computer (X): %d\n", comp_score);
    if (user_score > comp_score) {
        printf("Result: User Won! Congratulations!\n");
    } else if (comp_score > user_score) {
        printf("Result: Computer Won!\n");
    } else {
        printf("Result: It is a Tie!\n");
    }
    printf("===================================\n");

    free_board(board, size);
}

int main(void) {
    play_game();
    return 0;
}
