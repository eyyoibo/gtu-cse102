#include <stdio.h>

int main() {
    int choice; 
    int min_x0, min_y0, max_x0, max_y0; 
    int min_x1, min_y1, max_x1, max_y1; 

    min_x0 = min_y0 = max_x0 = max_y0 = 0;
    min_x1 = min_y1 = max_x1 = max_y1 = 0;

    while (1) { 
        printf("Menu:\n");
        printf("1) Explore Data\n");
        printf("2) Train Model\n");
        printf("3) Test Model\n");
        printf("4) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); 

        switch(choice) { // Switch case based on user choice
            case 1: { 
                FILE *file = fopen("data.txt", "r"); // Open file for reading

                int x, y, c; // Variables to read data from file

                while (fscanf(file, "%d %d %d", &x, &y, &c) != EOF) {
                    if (c == 0) { // Class 0 data
                        // Update min/max values for class 0
                        if (min_x0 == 0) min_x0 = x;
                        if (x < min_x0) min_x0 = x;
                        if (y < min_y0) min_y0 = y;
                        if (x > max_x0) max_x0 = x;
                        if (y > max_y0) max_y0 = y;
                    } 
                    else { // Class 1 data
                        // Update min/max values for class 1
                        if (min_x1 == 0) min_x1 = x;
                        if (x < min_x1) min_x1 = x;
                        if (y < min_y1) min_y1 = y;
                        if (x > max_x1) max_x1 = x;
                        if (y > max_y1) max_y1 = y;
                    }
                }
                fclose(file); 

                // Print min/max values for both classes
                printf("Minimum and Maximum X and Y values:\n");
                printf("Class 0: X in [%d %d], Y in [%d %d]\n", min_x0, max_x0, min_y0, max_y0);
                printf("Class 1: X in [%d %d], Y in [%d %d]\n", min_x1, max_x1, min_y1, max_y1);
                break;
            }
            case 2: 
                printf("Training model...\n");
                break;
            case 3: 
                printf("Testing model...\n");
                break;
            case 4: 
                printf("Exiting program...\n");
                return 0; // Exit program
            default: 
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }

    return 0;
}
