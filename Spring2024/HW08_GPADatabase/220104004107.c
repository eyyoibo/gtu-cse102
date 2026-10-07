#include <stdio.h>

// Function to calculate GPA
char calculate_gpa(FILE *info_fp, int id, int midterm_average, int final_average) {
    rewind(info_fp);
    int current_id, midterm, final;
    double midterm_gpa = 0.0, final_gpa = 0.0;

    while (fscanf(info_fp, "%d;%d;%d", &current_id, &midterm, &final) == 3) {
        if (current_id == id) {
            if (midterm < 40) {
                midterm_gpa = 1.0; // 'F'
            } else if (midterm < midterm_average) {
                midterm_gpa = 1.5; // 'D'
            } else if (midterm < 60) {
                midterm_gpa = 2.0; // 'C'
            } else if (midterm < 80) {
                midterm_gpa = 3.0; // 'B'
            } else {
                midterm_gpa = 4.0; // 'A'
            }

            if (final < 40) {
                final_gpa = 1.0; // 'F'
            } else if (final < final_average) {
                final_gpa = 1.5; // 'D'
            } else if (final < 60) {
                final_gpa = 2.0; // 'C'
            } else if (final < 80) {
                final_gpa = 3.0; // 'B'
            } else {
                final_gpa = 4.0; // 'A'
            }

            // Calculate GPA
            double gpa = (midterm_gpa + final_gpa) / 2.0;

            if (gpa >= 3.5) {
                return 'A';
            } else if (gpa >= 3.0) {
                return 'B';
            } else if (gpa >= 2.5) {
                return 'C';
            } else if (gpa >= 2.0) {
                return 'D';
            } else {
                return 'F';
            }
        }
    }

    return 'X'; // No grades found for the user
}


//pass or fail for each user
void print_pass_fail(FILE *info_fp) {
    rewind(info_fp);
    int id, midterm, final, course_id;
    while (fscanf(info_fp, "%d;%d;%d;%d;%d", &id, &midterm, &final, &course_id) == 5) {
        char result = (midterm + final) / 2 >= 40 ? 'P' : 'F';
        printf("ID: %d, Result: %c\n", id, result);
    }
}

// passed or failed the same class
void print_pass_fail_same_class(FILE *info_fp, int course_id) {
    rewind(info_fp);
    int id, midterm, final, course;

    while (fscanf(info_fp, "%d;%d;%d;%d;%d", &id, &midterm, &final, &course, &course_id) == 5) {
        if (course == course_id) {
            char result = ((midterm + final) / 2) >= 40 ? 'P' : 'F';
            printf("ID: %d, Result: %c\n", id, result);
        }
    }
}

void print_course_id(FILE *info_fp, int student_id) {
    rewind(info_fp);
    int id, midterm, final, course_id;

    while (fscanf(info_fp, "%d;%d;%d;%d", &id, &midterm, &final, &course_id) == 4) {
        if (id == student_id) {
            printf("Course ID: %d\n", course_id);
        }
    }
}



int main() {
    // Open the files
    FILE *id_fp = fopen("second 1.txt", "r");
    FILE *info_fp = fopen("first 1.txt", "r");

    // Check if files opened successfully
    if (id_fp == NULL || info_fp == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    char option;
    do {
        printf("\nMenu:\n");
        printf("p: Print all the users pass or fail.\n");
        printf("n: Print only the user with a specific first initial.(IT DOESN'T WORK)\n");
        printf("g: Calculate the GPA of a given student.\n");
        printf("c: Print whether each user passed or failed the same class.\n");
        printf("t: Print the number of classes for the instructor.(IT DOESN'T WORK)\n");
        printf("d: Print the department of all persons according to the role(IT DOESN'T WORK).\n");
        printf("l: Print the course_id of a given student.\n");
        printf("e: Quit the program.\n");
        printf("Enter your choice: ");
        scanf(" %c", &option);

        switch (option) {
            case 'p':
                printf("\nPrinting pass or fail for each user:\n");
                print_pass_fail(info_fp);
                break;
            case 'n': {
printf("\nIT DOESN'T WORK\n");
                break;
            }
            case 'g': {
                printf("\nEnter the user ID: ");
                int student_id;
                scanf("%d", &student_id);
                printf("\nCalculating GPA for student with ID %d:\n", student_id);
                // Specify your Midterm and Final averages here
                char gpa = calculate_gpa(info_fp, student_id, 80, 85);
                if (gpa != 'X') {
                    printf("GPA: %c\n", gpa);
                } else {
                    printf("No grades found for this user.\n");
                }
                break;
            }
case 'c': {
    printf("\nEnter the course ID: ");
    int course_id;
    scanf("%d", &course_id);
    printf("\nPrinting whether each user passed or failed the same class (Course ID: %d):\n", course_id);
    // Fonksiyon çağrısını ekleyin
    print_pass_fail_same_class(info_fp, course_id);
    break;
}
            case 't':
                printf("\nIT DOESN'T WORK\n");
                break;
            case 'd':
                printf("\nIT DOESN'T WORK\n");
                break;
case 'l': {
    printf("\nEnter the user ID: ");
    int student_id;
    scanf("%d", &student_id);
    printf("\nPrinting the course ID of the given student (Student ID: %d):\n", student_id);
    print_course_id(info_fp, student_id);
    break;
}
            case 'e':
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid option. Please try again.\n");
        }
    } while (option != 'e');

    fclose(id_fp);
    fclose(info_fp);

    return 0;
}
