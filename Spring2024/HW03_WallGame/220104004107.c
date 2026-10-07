#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 16 // Oyun tahtasının boyutu (16x16)
#define WALL_SIZE 2 // Belirli alanların etrafına yerleştirilen duvarların genişliği
#define FIRST_YEAR_ECTS_REQUIRED 32 // Birinci yıl geçmek için gereken toplam ECTS
#define SECOND_YEAR_ECTS_REQUIRED 24 // İkinci yıl geçmek için gereken toplam ECTS
#define FIRST_YEAR_COURSE '1' // Birinci yıl derslerini temsil eden sembol
#define SECOND_YEAR_COURSE '2' // İkinci yıl derslerini temsil eden sembol
#define STUDENT 'P'
#define EMPTY_SPACE '.'
#define TARGET_SYMBOL 'X' // Hedef bölge
#define WALL_SYMBOL '#' // Duvarsembolü


char board[BOARD_SIZE * BOARD_SIZE];
int studentPos;       // Oyuncunun tahtadaki konumu
int totalEcts = 0;
int totalMoves = 0;
int secondYearCourseCount = 0;

void initializeGame()
{
    // Tahtayı . lar ile başlatma
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        board[i] = EMPTY_SPACE;
    }

    // İkinci yıl derslerini rasgele tahtaya yerleştirme
    int secondYearCoursePlaced = 0;
    while (secondYearCoursePlaced < 3) {
        int position = rand() % (BOARD_SIZE * BOARD_SIZE);
        if (board[position] == EMPTY_SPACE) {
            int row = position / BOARD_SIZE;
            int col = position % BOARD_SIZE;
            if (row >= WALL_SIZE && row < BOARD_SIZE - WALL_SIZE && col >= WALL_SIZE && col < BOARD_SIZE - WALL_SIZE) {
                board[position] = SECOND_YEAR_COURSE;
                secondYearCoursePlaced++;
            }
        }
    }

    // 5x5 olusturma
    int startRow = (BOARD_SIZE / 2) - 2;
    int startCol = (BOARD_SIZE / 2) - 2;
    int oneCount = 0;

    // 5 x 5 duvarların içine 1 leri yerleştirme
    for (int i = startRow; i < startRow + 5; ++i) {
        for (int j = startCol; j < startCol + 5; ++j) {
            if (i == startRow || i == startRow + 4 || j == startCol || j == startCol + 4) {
                board[i * BOARD_SIZE + j] = WALL_SYMBOL;
            } else {
                if (oneCount < 4) {
                    board[i * BOARD_SIZE + j] = FIRST_YEAR_COURSE;
                    oneCount++;
                } else {
                    board[i * BOARD_SIZE + j] = EMPTY_SPACE;
                }
            }
        }
    }

    // 9x9 kareyi yerlestirme
    int wallStartRow = (BOARD_SIZE / 2) - 4;
    int wallStartCol = (BOARD_SIZE / 2) - 4;

    for (int i = wallStartRow; i < wallStartRow + 9; ++i) {
        for (int j = wallStartCol; j < wallStartCol + 9; ++j) {
            if (i == wallStartRow || i == wallStartRow + 8 || j == wallStartCol || j == wallStartCol + 8) {
                board[i * BOARD_SIZE + j] = WALL_SYMBOL;
            }
        }
    }

    // oyuncu 'P'
    studentPos = BOARD_SIZE * (BOARD_SIZE / 2) + (BOARD_SIZE / 2);
    board[studentPos] = STUDENT;

    // İkinci yıl derslerini rasgele tahtaya yerleştirme
    for (int i = 0; i < 3; ++i) {
        int position;
        do {
            position = rand() % (BOARD_SIZE * BOARD_SIZE);
        } while (board[position] != EMPTY_SPACE);
        board[position] = SECOND_YEAR_COURSE;
    }

    // X yerlestırme
    board[BOARD_SIZE * BOARD_SIZE - 1] = TARGET_SYMBOL;
}

void printBoard()
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%c ", board[i * BOARD_SIZE + j]);
        }
        printf("\n");
    }
}

void movePlayer(char direction)
{
    int newPos = studentPos;
    switch (direction) {
    case 'a':
        if (studentPos % BOARD_SIZE != 0)
            newPos--;
        break;
    case 'd':
        if (studentPos % BOARD_SIZE != BOARD_SIZE - 1)
            newPos++;
        break;
    case 'w':
        if (studentPos >= BOARD_SIZE)
            newPos -= BOARD_SIZE;
        break;
    case 's':
        if (studentPos < BOARD_SIZE * (BOARD_SIZE - 1))
            newPos += BOARD_SIZE;
        break;
    default:
        printf("Invalid move!\n");
        return;
    }

    if (board[newPos] == EMPTY_SPACE) {
        // Boş bir alana hareket etme
        totalMoves++;
        board[studentPos] = EMPTY_SPACE;
        studentPos = newPos;
        board[studentPos] = STUDENT;
    } else if (board[newPos] == FIRST_YEAR_COURSE || board[newPos] == SECOND_YEAR_COURSE) {
        // Bir ders toplama
        totalMoves++;
        totalEcts += 8;
        board[studentPos] = EMPTY_SPACE;
        studentPos = newPos;
        board[studentPos] = STUDENT;
        if (board[newPos] == SECOND_YEAR_COURSE) {
            secondYearCourseCount++;
        }
    } else if (board[newPos] == TARGET_SYMBOL) {
        // Hedef sembolüne ulaşma
        if (totalEcts >= FIRST_YEAR_ECTS_REQUIRED + SECOND_YEAR_ECTS_REQUIRED) {
            printf("\nCongrats! You reached 'X'\n");
            exit(0);
        } else {
            printf("Not enough ECTS collected to reach the target!\n");
        }
    } else if (board[newPos] == WALL_SYMBOL) {
        // Bir duvarı geçmeye çalışma
        if (totalEcts >= FIRST_YEAR_ECTS_REQUIRED) {
            // Toplam ECTS'nin duvarı geçmek için yeterli olup olmadığını kontrol etme
            totalMoves++;
            board[studentPos] = EMPTY_SPACE;
            studentPos = newPos;
            board[studentPos] = STUDENT;
        } else {
            printf("There are not enough ECTS to cross the wall!\n");
        }
    }
}

int main()
{
    srand(time(NULL));

    initializeGame();

    char direction;
    while (1) {
        printf("\nW (UP), A (LEFT), S (DOWN), D (RIGHT) to move\n");
        printBoard();
        printf("\nTotal ECTS: %d", totalEcts);
        printf("\nEnter your move:");
        scanf(" %c", &direction);
        movePlayer(direction);
        printf("\n");


        if (totalEcts >= FIRST_YEAR_ECTS_REQUIRED + SECOND_YEAR_ECTS_REQUIRED &&
                studentPos == BOARD_SIZE * BOARD_SIZE - 1) {
            printf("\nCongrats! You reached 'X'\n");
            break;
        }
    }

    return 0;
}