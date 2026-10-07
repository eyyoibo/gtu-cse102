#include <stdio.h>

// Function to print the contents of the news file based on news number
void printNewContext(int news_number) {
    FILE *file;
    
    // Open the file based on the news number
    if (news_number == 1) {
        file = fopen("1.txt", "r");
    } else if (news_number == 2) {
        file = fopen("2.txt", "r");
    } else if (news_number == 3) {
        file = fopen("3.txt", "r");
    } else if (news_number == 4) {
        file = fopen("4.txt", "r");
    } else {
        printf("Type number between 1-4.\n");
        return;
    }

    // Print the contents of the file
    int i;
    while ((i = fgetc(file)) != EOF) {
        printf("%c", i); 
    }

    fclose(file);
}

// Function to print the news that have already been read
void printReadedNews() {
    printf("Readed news are listed below:\n");

    // Open the file containing the number of news of read news
    FILE *read_news_file = fopen("readed_news_id.txt", "r");
    if (read_news_file == NULL) {
        printf("No news readed yet.\n");
        return;
    }

    // Read and print the number of news of read news
    int read_news_id;
    while (fscanf(read_news_file, "%d", &read_news_id) != EOF) {
        printf("%d. news are readed\n", read_news_id);
    }

    fclose(read_news_file);
}

// Function to check if a news has already been read
int isNewsAlreadyRead(int news_number) {
    // Open the file containing the number of news of read news
    FILE *read_news_file = fopen("readed_news_id.txt", "r");

    // Check if the news number is found in the file
    int read_news_id;
    while (fscanf(read_news_file, "%d", &read_news_id) != EOF) {
        if (read_news_id == news_number) {
            fclose(read_news_file);
            return 1; 
        }
    }

    fclose(read_news_file);
    return 0; 
}

// Function to display the menu and handle user interactions
void menu() {
    char choice;
    do {
        printf("**************Daily Press**************\n");
        printf("Today's news listed for you:\n");
        printf("1. news: Astronot Alper Gezeravcı'nın Dünya'ya Dönüşü Ertelendi\n");
        printf("2. news: Bakan Kacır Astronot Gezeravcı ile Videokonferans Görüşmesi Gerçekleştirdi\n");
        printf("3. news: Alper Gezeravcı uzayda 9. deneyini yaptı\n");
        printf("4. news: Astronotların uzaydaki ilk sözleri\n");
        printf("What do you want to do?\n");
        printf("a. Read a new\n");
        printf("b. List the read news\n");
        printf("c. Get encrypted information from the news (IT DOESN'T WORKING)\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'a':
                printf("Which news do you want to read (1-4): ");
                scanf(" %c", &choice);
                printf("You chose to read news %c\n", choice);
                if (isNewsAlreadyRead(choice - '0')) {
                    printf("This news has already been read. Do you want to read again? Yes (1) / No (0): ");
                    int readAgainChoice;
                    scanf("%d", &readAgainChoice);
                    if (readAgainChoice == 1) {
                        printNewContext(choice - '0');
                    }
                } else {
                    printNewContext(choice - '0');
                    
                    // Record the number of news of the newly read news
                    FILE *read_news_file = fopen("readed_news_id.txt", "a");
                    fprintf(read_news_file, "%c\n", choice);
                    fclose(read_news_file);
                }
                break;
            case 'b':
                printReadedNews(); 
                break;
            case 'c':
                printf("IT DOESN'T WORKING\n");
                break;
            default:
                printf("Please type between a-c\n");
        }

        printf("Do you want to continue? Yes (y) / No (n): ");
        scanf(" %c", &choice);
    } while (choice == 'y'); 

    printf("Good Bye!\n");
}

int main() {
    menu();
    return 0;
}
