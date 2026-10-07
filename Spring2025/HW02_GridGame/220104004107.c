#include <stdio.h>
#include <time.h>

int main() {
    FILE *dosya;
    int width, height, playerX, playerY, doorX, doorY;
    char move;

    dosya = fopen("game_state.txt", "r");

    if (dosya == NULL) {
        printf("Generating a new board...\n");
        printf("Enter the width and height of board: ");
        scanf("%d %d", &width, &height);

        int t = time(NULL); 

        playerX = t % width;
        playerY = (t / 2) % height;
        doorX = (t / 3) % width;
        doorY = (t / 4) % height;

        dosya = fopen("game_state.txt", "w");
        fprintf(dosya, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
        fclose(dosya);
    } else {
        fscanf(dosya, "%d %d %d %d %d %d", &width, &height, &playerX, &playerY, &doorX, &doorY);
        fclose(dosya);
    }

    printf("Width: %d, Height: %d Player: (%d,%d), Door: (%d,%d)\n", width, height, playerX, playerY, doorX, doorY);
    printf("Enter move (WASD): ");
    scanf(" %c", &move);

    if (move == 'W' || move == 'w') {
        playerY--;
        printf("Player moves up to (%d, %d)\n", playerX, playerY);
    } else if (move == 'S' || move == 's') {
        playerY++;
        printf("Player moves down to (%d, %d)\n", playerX, playerY);
    } else if (move == 'A' || move == 'a') {
        playerX--;
        printf("Player moves left to (%d, %d)\n", playerX, playerY);
    } else if (move == 'D' || move == 'd') {
        playerX++;
        printf("Player moves right to (%d, %d)\n", playerX, playerY);
    } else {
        printf("Invalid key, please enter W, A, S, or D\n");
        return 0;
    }

    // Boundries check
    if (playerX < 0 || playerX >= width || playerY < 0 || playerY >= height) {
        printf("You hit the wall!\n");
        return 0;
    }

    // Win condition
    if (playerX == doorX && playerY == doorY) {
        printf("Congratulations! You escaped!\n");
        remove("game_state.txt");
    } else {
        dosya = fopen("game_state.txt", "w");
        fprintf(dosya, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
        fclose(dosya);
        printf("Game continues...\n");
    }

    return 0;
}
