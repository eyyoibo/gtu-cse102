#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define MAX_COURSES 5

typedef enum {
    CSE101 = 0,
    CSE102 = 1,
    CSE103 = 2,
    CSE104 = 3,
    CSE105 = 4,
    UNKNOWN_COURSE = -1
} LectureCode;

typedef struct {
    LectureCode code;
    int grade;
} CourseEnrollment;

typedef struct Student {
    int id;
    char name[MAX_NAME_LEN];
    CourseEnrollment courses[MAX_COURSES];
    int num_courses;
    double gpa;
    struct Student *next;
} Student;

static int g_next_id = 1;

int get_course_credits(LectureCode code) {
    switch (code) {
        case CSE101: return 2;
        case CSE102: return 3;
        case CSE103: return 3;
        case CSE104: return 4;
        case CSE105: return 5;
        default: return 0;
    }
}

const char* get_course_name(LectureCode code) {
    switch (code) {
        case CSE101: return "CSE101";
        case CSE102: return "CSE102";
        case CSE103: return "CSE103";
        case CSE104: return "CSE104";
        case CSE105: return "CSE105";
        default: return "UNKNOWN";
    }
}

LectureCode parse_course_name(const char *name) {
    char clean[32];
    int i, ci = 0;
    for (i = 0; name[i] != '\0' && ci < 31; i++) {
        if (!isspace((unsigned char)name[i])) {
            clean[ci++] = (char)toupper((unsigned char)name[i]);
        }
    }
    clean[ci] = '\0';

    if (strcmp(clean, "CSE101") == 0) return CSE101;
    if (strcmp(clean, "CSE102") == 0) return CSE102;
    if (strcmp(clean, "CSE103") == 0) return CSE103;
    if (strcmp(clean, "CSE104") == 0) return CSE104;
    if (strcmp(clean, "CSE105") == 0) return CSE105;
    return UNKNOWN_COURSE;
}

void calculate_gpa(Student *s) {
    int i;
    int total_credits = 0;
    double weighted_sum = 0.0;

    if (s->num_courses == 0) {
        s->gpa = 0.0;
        return;
    }

    for (i = 0; i < s->num_courses; i++) {
        int credits = get_course_credits(s->courses[i].code);
        weighted_sum += s->courses[i].grade * credits;
        total_credits += credits;
    }

    if (total_credits > 0) {
        s->gpa = weighted_sum / (double)total_credits;
    } else {
        s->gpa = 0.0;
    }
}

int split_comma_separated(const char *input, char tokens[][MAX_NAME_LEN], int max_tokens) {
    int count = 0;
    const char *p = input;
    while (*p && count < max_tokens) {
        const char *start = p;
        int len;
        while (*p && *p != ',') p++;
        len = (int)(p - start);
        if (len >= MAX_NAME_LEN) len = MAX_NAME_LEN - 1;
        strncpy(tokens[count], start, len);
        tokens[count][len] = '\0';
        count++;
        if (*p == ',') p++;
    }
    return count;
}

void print_all_ids(Student *head) {
    Student *curr = head;
    printf("id : ");
    while (curr) {
        printf("%d", curr->id);
        if (curr->next) {
            printf(",");
        }
        curr = curr->next;
    }
    printf("\n");
}

Student* search_student(Student *head, int id) {
    Student *curr = head;
    while (curr) {
        if (curr->id == id) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void print_student_details(Student *s) {
    int i;
    printf("-------------------------------------------------------------------------------------------\n");
    printf("%-15s %-30s %-20s %-10s\n", "Name", "Lectures", "Grades", "GPA");
    printf("-------------------------------------------------------------------------------------------\n");

    printf("%-15s ", s->name);
    for (i = 0; i < s->num_courses; i++) {
        printf("%s%s", get_course_name(s->courses[i].code), (i < s->num_courses - 1) ? ", " : "");
    }
    if (s->num_courses == 0) printf("None");
    printf(" ");

    for (i = 0; i < s->num_courses; i++) {
        printf("%d%s", s->courses[i].grade, (i < s->num_courses - 1) ? ", " : "");
    }
    if (s->num_courses == 0) printf("None");
    printf(" ");

    printf("%.2f\n", s->gpa);
}

void print_full_list(Student *head) {
    Student *curr = head;
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-30s %-20s %-10s %s\n", "Name", "Lectures", "Grades", "GPA", "id");
    printf("----------------------------------------------------------------------------------------------------\n");

    while (curr) {
        int i;
        char lect_buf[128] = "";
        char grad_buf[128] = "";

        for (i = 0; i < curr->num_courses; i++) {
            char temp[32];
            strcat(lect_buf, get_course_name(curr->courses[i].code));
            sprintf(temp, "%d", curr->courses[i].grade);
            strcat(grad_buf, temp);
            if (i < curr->num_courses - 1) {
                strcat(lect_buf, ", ");
                strcat(grad_buf, ", ");
            }
        }
        if (curr->num_courses == 0) {
            strcpy(lect_buf, "None");
            strcpy(grad_buf, "None");
        }

        printf("%-15s %-30s %-20s %-10.2f %d\n",
               curr->name, lect_buf, grad_buf, curr->gpa, curr->id);
        curr = curr->next;
    }
}

Student* add_student(Student *head, const char *name, const char *lectures_str, const char *grades_str) {
    Student *new_node = (Student*)malloc(sizeof(Student));
    char lect_toks[MAX_COURSES][MAX_NAME_LEN];
    char grad_toks[MAX_COURSES][MAX_NAME_LEN];
    int n_lect, n_grad, i;

    if (!new_node) {
        perror("Allocation failure");
        exit(EXIT_FAILURE);
    }

    new_node->id = g_next_id++;
    strncpy(new_node->name, name, MAX_NAME_LEN - 1);
    new_node->name[MAX_NAME_LEN - 1] = '\0';
    new_node->num_courses = 0;
    new_node->next = NULL;

    n_lect = split_comma_separated(lectures_str, lect_toks, MAX_COURSES);
    n_grad = split_comma_separated(grades_str, grad_toks, MAX_COURSES);

    for (i = 0; i < n_lect && i < n_grad && new_node->num_courses < MAX_COURSES; i++) {
        LectureCode lc = parse_course_name(lect_toks[i]);
        if (lc != UNKNOWN_COURSE) {
            new_node->courses[new_node->num_courses].code = lc;
            new_node->courses[new_node->num_courses].grade = atoi(grad_toks[i]);
            new_node->num_courses++;
        }
    }

    calculate_gpa(new_node);

    if (!head) {
        return new_node;
    } else {
        Student *curr = head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_node;
        return head;
    }
}

Student* remove_student(Student *head, int id, int *removed) {
    Student *curr = head;
    Student *prev = NULL;
    *removed = 0;

    while (curr) {
        if (curr->id == id) {
            *removed = 1;
            if (prev == NULL) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return head;
        }
        prev = curr;
        curr = curr->next;
    }
    return head;
}

void add_lecture_to_student(Student *s, const char *lectures_str, const char *grades_str) {
    char lect_toks[MAX_COURSES][MAX_NAME_LEN];
    char grad_toks[MAX_COURSES][MAX_NAME_LEN];
    int n_lect = split_comma_separated(lectures_str, lect_toks, MAX_COURSES);
    int n_grad = split_comma_separated(grades_str, grad_toks, MAX_COURSES);
    int idx;

    for (idx = 0; idx < n_lect && idx < n_grad; idx++) {
        LectureCode lc = parse_course_name(lect_toks[idx]);
        int gr = atoi(grad_toks[idx]);
        if (lc != UNKNOWN_COURSE) {
            int i, found = 0;
            for (i = 0; i < s->num_courses; i++) {
                if (s->courses[i].code == lc) {
                    s->courses[i].grade = gr;
                    found = 1;
                    break;
                }
            }
            if (!found && s->num_courses < MAX_COURSES) {
                s->courses[s->num_courses].code = lc;
                s->courses[s->num_courses].grade = gr;
                s->num_courses++;
            }
        }
    }
    calculate_gpa(s);
}

void remove_lecture_from_student(Student *s, const char *lecture_name) {
    LectureCode lc = parse_course_name(lecture_name);
    int i, j;
    if (lc == UNKNOWN_COURSE) return;

    for (i = 0; i < s->num_courses; i++) {
        if (s->courses[i].code == lc) {
            for (j = i; j < s->num_courses - 1; j++) {
                s->courses[j] = s->courses[j + 1];
            }
            s->num_courses--;
            break;
        }
    }
    calculate_gpa(s);
}

void free_all_students(Student *head) {
    Student *curr = head;
    while (curr) {
        Student *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
}

void run_menu(void) {
    Student *head = NULL;
    char line[256];
    int choice = 0;

    while (1) {
        printf("\nMenu\n");
        printf("1. Add Student\n");
        printf("2. Remove Student\n");
        printf("3. Search Student\n");
        printf("4. Add Lecture\n");
        printf("5. Remove Lecture\n");
        printf("6. Print ID\n");
        printf("7. Print List\n");
        printf("8. Exit\n");
        printf("Choice: ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;
        if (sscanf(line, "%d", &choice) != 1) continue;

        if (choice == 1) {
            char name[MAX_NAME_LEN];
            char lect[256];
            char grad[256];

            printf("Enter student name: ");
            fflush(stdout);
            if (!fgets(name, sizeof(name), stdin)) break;
            name[strcspn(name, "\r\n")] = '\0';

            printf("Enter lectures: ");
            fflush(stdout);
            if (!fgets(lect, sizeof(lect), stdin)) break;
            lect[strcspn(lect, "\r\n")] = '\0';

            printf("Enter grades: ");
            fflush(stdout);
            if (!fgets(grad, sizeof(grad), stdin)) break;
            grad[strcspn(grad, "\r\n")] = '\0';

            head = add_student(head, name, lect, grad);
            printf("Student added.\n");
            print_all_ids(head);

        } else if (choice == 2) {
            int id, removed;
            printf("Enter student id: ");
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            id = atoi(line);
            head = remove_student(head, id, &removed);
            if (removed) {
                printf("Student was removed.\n");
                print_all_ids(head);
            } else {
                printf("Student not found.\n");
            }

        } else if (choice == 3) {
            int id;
            Student *s;
            printf("Enter student id: ");
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            id = atoi(line);
            s = search_student(head, id);
            if (s) {
                print_student_details(s);
            } else {
                printf("Student not found.\n");
            }

        } else if (choice == 4) {
            int id;
            char lect[256];
            char grad[256];
            Student *s;

            printf("Enter student id: ");
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            id = atoi(line);
            s = search_student(head, id);
            if (!s) {
                printf("Student not found.\n");
                continue;
            }

            printf("Enter lectures: ");
            fflush(stdout);
            if (!fgets(lect, sizeof(lect), stdin)) break;
            lect[strcspn(lect, "\r\n")] = '\0';

            printf("Enter grades: ");
            fflush(stdout);
            if (!fgets(grad, sizeof(grad), stdin)) break;
            grad[strcspn(grad, "\r\n")] = '\0';

            add_lecture_to_student(s, lect, grad);
            print_student_details(s);

        } else if (choice == 5) {
            int id;
            char lect[256];
            Student *s;

            printf("Enter student id: ");
            fflush(stdout);
            if (!fgets(line, sizeof(line), stdin)) break;
            id = atoi(line);
            s = search_student(head, id);
            if (!s) {
                printf("Student not found.\n");
                continue;
            }

            printf("Enter lectures: ");
            fflush(stdout);
            if (!fgets(lect, sizeof(lect), stdin)) break;
            lect[strcspn(lect, "\r\n")] = '\0';

            remove_lecture_from_student(s, lect);
            print_student_details(s);

        } else if (choice == 6) {
            print_all_ids(head);

        } else if (choice == 7) {
            print_full_list(head);

        } else if (choice == 8) {
            printf("Freeing memory... Have a nice day!\n");
            free_all_students(head);
            break;
        }
    }
}

int main(void) {
    run_menu();
    return 0;
}
