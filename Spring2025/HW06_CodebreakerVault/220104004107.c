#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // Still needed for time()

// Struct to store game configuration from vault_config.txt
struct Config {
    int code_length;
    int digit_min;
    int digit_max;
    int allow_duplicates;
    int max_attempts;
    int points_correct;
    int points_misplaced;
    int penalty_wrong;
};

// Helper function to check if a digit is duplicated in the array
int is_duplicate(int *array, int length, int value) {
    for (int i = 0; i < length; i++)
        if (array[i] == value) return 1;
    return 0;
}

// Reads configuration settings from vault_config.txt
struct Config read_config() {
    FILE *file = fopen("vault_config.txt", "r");
    if (!file) {
        printf("Error: vault_config.txt not found.\n");
        exit(1);
    }
    struct Config cfg;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "CODE_LENGTH=%d", &cfg.code_length)) continue;
        if (sscanf(line, "DIGIT_MIN=%d", &cfg.digit_min)) continue;
        if (sscanf(line, "DIGIT_MAX=%d", &cfg.digit_max)) continue;
        if (sscanf(line, "ALLOW_DUPLICATES=%d", &cfg.allow_duplicates)) continue;
        if (sscanf(line, "MAX_ATTEMPTS=%d", &cfg.max_attempts)) continue;
        if (sscanf(line, "POINTS_CORRECT=%d", &cfg.points_correct)) continue;
        if (sscanf(line, "POINTS_MISPLACED=%d", &cfg.points_misplaced)) continue;
        if (sscanf(line, "PENALTY_WRONG=%d", &cfg.penalty_wrong)) continue;
    }
    fclose(file);
    return cfg;
}

// Generates a secret code based on configuration
int* generate_code() {
    struct Config config = read_config();
    FILE *file = fopen("vault_code.txt", "r");
    int *code = malloc(config.code_length * sizeof(int));

    if (file) {
        char buffer[100];
        fgets(buffer, sizeof(buffer), file);
        fclose(file);
        for (int i = 0; i < config.code_length; i++) {
            code[i] = buffer[i] - '0';
        }
        return code;
    }

    file = fopen("vault_code.txt", "w");
    srand(time(NULL));
    for (int i = 0; i < config.code_length; i++) {
        int digit;
        do {
            digit = rand() % (config.digit_max - config.digit_min + 1) + config.digit_min;
        } while (!config.allow_duplicates && is_duplicate(code, i, digit));
        code[i] = digit;
        fprintf(file, "%d", digit);
    }
    fprintf(file, "\n");
    fclose(file);
    return code;
}

// Prompts the user to enter a valid guess and returns it
int* get_guess() {
    struct Config config = read_config();
    char input[100];
    int *guess = malloc(config.code_length * sizeof(int));
    int valid;

    do {
        valid = 1;
        printf("Enter your guess: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if ((int)strlen(input) != config.code_length) {
            printf("Invalid length! Must be %d digits.\n", config.code_length);
            valid = 0;
            continue;
        }

        for (int i = 0; i < config.code_length; i++) {
            if (input[i] < '0' || input[i] > '9') {
                printf("Invalid character '%c'.\n", input[i]);
                valid = 0;
                break;
            }
            guess[i] = input[i] - '0';
            if (guess[i] < config.digit_min || guess[i] > config.digit_max) {
                printf("Digit out of range (%d–%d).\n", config.digit_min, config.digit_max);
                valid = 0;
                break;
            }
        }
    } while (!valid);

    return guess;
}

// Generates feedback string (C, M, W) based on user's guess
char* calculate_feedback(int *code, int *guess, int length) {
    char *feedback = malloc(length + 1);
    int *code_used = calloc(length, sizeof(int));
    int *guess_used = calloc(length, sizeof(int));

    for (int i = 0; i < length; i++) {
        if (guess[i] == code[i]) {
            feedback[i] = 'C';
            code_used[i] = guess_used[i] = 1;
        } else {
            feedback[i] = ' ';
        }
    }

    for (int i = 0; i < length; i++) {
        if (guess_used[i]) continue;
        for (int j = 0; j < length; j++) {
            if (!code_used[j] && guess[i] == code[j]) {
                feedback[i] = 'M';
                code_used[j] = guess_used[i] = 1;
                break;
            }
        }
    }

    for (int i = 0; i < length; i++) {
        if (feedback[i] == ' ') feedback[i] = 'W';
    }

    feedback[length] = '\0';
    free(code_used);
    free(guess_used);
    return feedback;
}

// Allows the user to create and save a new game configuration
void admin_mode() {
    struct Config config;
    printf("Enter code length: ");
    scanf("%d", &config.code_length);
    while (getchar() != '\n');
    printf("Enter digit min: ");
    scanf("%d", &config.digit_min);
    while (getchar() != '\n');
    printf("Enter digit max: ");
    scanf("%d", &config.digit_max);
    while (getchar() != '\n');
    printf("Allow duplicates? (0 = No, 1 = Yes): ");
    scanf("%d", &config.allow_duplicates);
    while (getchar() != '\n');
    printf("Enter max attempts: ");
    scanf("%d", &config.max_attempts);
    while (getchar() != '\n');

    config.points_correct = 10;
    config.points_misplaced = 5;
    config.penalty_wrong = 2;

    FILE *file = fopen("vault_config.txt", "w");
    fprintf(file, "CODE_LENGTH=%d\n", config.code_length);
    fprintf(file, "DIGIT_MIN=%d\n", config.digit_min);
    fprintf(file, "DIGIT_MAX=%d\n", config.digit_max);
    fprintf(file, "ALLOW_DUPLICATES=%d\n", config.allow_duplicates);
    fprintf(file, "MAX_ATTEMPTS=%d\n", config.max_attempts);
    fprintf(file, "POINTS_CORRECT=%d\n", config.points_correct);
    fprintf(file, "POINTS_MISPLACED=%d\n", config.points_misplaced);
    fprintf(file, "PENALTY_WRONG=%d\n", config.penalty_wrong);
    fclose(file);
    remove("vault_code.txt");
    printf("Configuration saved to vault_config.txt\n");
}

// Main game loop for the player
void player_mode() {
    struct Config config = read_config();
    int *code = generate_code();
    int total_score = 0, attempt = 0, win = 0;

    FILE *log = fopen("game_log.txt", "w");
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log, "--- Vault Codebreaker Game Log ---\n");
    fprintf(log, "Game Date: %s\n", time_str);
    fprintf(log, "Secret Code: ");
    for (int i = 0; i < config.code_length; i++) fprintf(log, "%d", code[i]);
    fprintf(log, "\nCode Length: %d\nDigit Range: %d-%d\nDuplicates Allowed: %d\nMax Attempts: %d\n\n",
        config.code_length, config.digit_min, config.digit_max, config.allow_duplicates, config.max_attempts);
    fclose(log);

    while (attempt < config.max_attempts && !win) {
        printf("\nAttempt %d/%d\n", attempt + 1, config.max_attempts);
        int *guess = get_guess();
        char *feedback = calculate_feedback(code, guess, config.code_length);

        int C = 0, M = 0, W = 0;
        for (int i = 0; i < config.code_length; i++) {
            if (feedback[i] == 'C') C++;
            else if (feedback[i] == 'M') M++;
            else if (feedback[i] == 'W') W++;
        }

        int attempt_score = (C * config.points_correct) + (M * config.points_misplaced) - (W * config.penalty_wrong);
        total_score += attempt_score;

        printf("Guess:    ");
        for (int i = 0; i < config.code_length; i++) printf("%d   ", guess[i]);
        printf("\n");

        printf("Feedback: ");
        for (int i = 0; i < config.code_length; i++) printf("%c   ", feedback[i]);
        printf("| Score: %d\n", total_score);

        log = fopen("game_log.txt", "a");
        fprintf(log, "Attempt %d: ", attempt + 1);
        for (int i = 0; i < config.code_length; i++) fprintf(log, "%d", guess[i]);
        fprintf(log, " => Feedback: ");
        for (int i = 0; i < config.code_length; i++) fprintf(log, "%c ", feedback[i]);
        fprintf(log, "| Total Score: %d\n", total_score);
        fclose(log);

        if (C == config.code_length) win = 1;

        free(guess);
        free(feedback);
        attempt++;
    }

    char *rank = total_score >= 90 ? "Code Master 🧠" :
                 total_score >= 70 ? "Cipher Hunter 💷" :
                 total_score >= 50 ? "Number Sleuth 💸" :
                 total_score >= 30 ? "Safe Kicker 💹" :
                 total_score >= 10 ? "Lucky Breaker 💻" : "Code Potato 💼";

    log = fopen("game_log.txt", "a");
    fprintf(log, "\nFinal Score: %d\nRank: %s\n", total_score, rank);
    fprintf(log, "-----------------------------------\n");
    fclose(log);

    printf("\nFinal Score: %d\nRank: %s\n", total_score, rank);
    free(code);
}

int main() {
    char mode;
    printf("Enter mode (A for Admin, P for Player): ");
    scanf(" %c", &mode);
    while (getchar() != '\n');

    switch (mode) {
        case 'A':
        case 'a':
            admin_mode();
            break;
        case 'P':
        case 'p':
            player_mode();
            break;
        default:
            printf("Invalid mode.\n");
            break;
    }

    return 0;
}
