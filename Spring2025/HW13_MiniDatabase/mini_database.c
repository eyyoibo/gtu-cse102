#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 100

typedef enum {
    BACKEND_DEVELOPER = 0,
    FRONTEND_DEVELOPER = 1,
    FULLSTACK_DEVELOPER = 2,
    MOBILE_DEVELOPER = 3,
    EMBEDDED_SOFTWARE_ENGINEER = 4,
    GAME_DEVELOPER = 5,
    DEVOPS_ENGINEER = 6,
    TEST_ENGINEER = 7
} JobType;

typedef struct {
    int id;
    char name[MAX_STR];
    char email[MAX_STR];
    JobType job_type;
    int is_active; /* 1: true, 0: false */
    char birth_date[MAX_STR];
} Customer;

typedef struct {
    Customer *records;
    int count;
    int capacity;
    int next_id;
} Database;

const char* job_type_to_str(JobType jt) {
    switch (jt) {
        case BACKEND_DEVELOPER: return "BACKEND_DEVELOPER";
        case FRONTEND_DEVELOPER: return "FRONTEND_DEVELOPER";
        case FULLSTACK_DEVELOPER: return "FULLSTACK_DEVELOPER";
        case MOBILE_DEVELOPER: return "MOBILE_DEVELOPER";
        case EMBEDDED_SOFTWARE_ENGINEER: return "EMBEDDED_SOFTWARE_ENGINEER";
        case GAME_DEVELOPER: return "GAME_DEVELOPER";
        case DEVOPS_ENGINEER: return "DEVOPS_ENGINEER";
        case TEST_ENGINEER: return "TEST_ENGINEER";
        default: return "BACKEND_DEVELOPER";
    }
}

JobType int_to_job_type(int val) {
    if (val >= 0 && val <= 7) return (JobType)val;
    return BACKEND_DEVELOPER;
}

void init_db(Database *db) {
    db->capacity = 16;
    db->count = 0;
    db->next_id = 1;
    db->records = (Customer*)malloc(db->capacity * sizeof(Customer));
    if (!db->records) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
}

void free_db(Database *db) {
    if (db->records) {
        free(db->records);
        db->records = NULL;
    }
    db->count = 0;
    db->capacity = 0;
}

void truncate_db(Database *db) {
    db->count = 0;
    db->next_id = 1;
}

void ensure_capacity(Database *db) {
    if (db->count >= db->capacity) {
        db->capacity *= 2;
        db->records = (Customer*)realloc(db->records, db->capacity * sizeof(Customer));
        if (!db->records) {
            perror("Reallocation error");
            exit(EXIT_FAILURE);
        }
    }
}

void add_customer(Database *db, const char *name, const char *email, JobType job, int active, const char *birth) {
    Customer *c;
    ensure_capacity(db);
    c = &db->records[db->count];
    c->id = db->next_id++;
    strncpy(c->name, name, MAX_STR - 1);
    c->name[MAX_STR - 1] = '\0';
    strncpy(c->email, email, MAX_STR - 1);
    c->email[MAX_STR - 1] = '\0';
    c->job_type = job;
    c->is_active = active;
    strncpy(c->birth_date, birth, MAX_STR - 1);
    c->birth_date[MAX_STR - 1] = '\0';
    db->count++;
}

int delete_customer(Database *db, int id) {
    int i, j;
    int found = 0;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].id == id) {
            found = 1;
            for (j = i; j < db->count - 1; j++) {
                db->records[j] = db->records[j + 1];
            }
            db->count--;
            i--;
        }
    }
    return found;
}

int update_customer(Database *db, int id, const char *new_email, int has_job, JobType new_job) {
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].id == id) {
            if (new_email && strlen(new_email) > 0) {
                strncpy(db->records[i].email, new_email, MAX_STR - 1);
                db->records[i].email[MAX_STR - 1] = '\0';
            }
            if (has_job) {
                db->records[i].job_type = new_job;
            }
            return 1;
        }
    }
    return 0;
}

void print_table(Database *db, FILE *out) {
    int i;
    for (i = 0; i < db->count; i++) {
        Customer *c = &db->records[i];
        fprintf(out, "%s,%s,%s,%s,%s\n",
                c->name,
                c->email,
                job_type_to_str(c->job_type),
                c->is_active ? "true" : "false",
                c->birth_date);
    }
    fprintf(out, "----------\n");
}

void clean_token(char *token) {
    char clean[MAX_STR];
    int ci = 0;
    int i;
    for (i = 0; token[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)token[i];
        /* Skip quotes (ASCII or UTF-8 quotes like \xe2\x80\x9c / \xe2\x80\x9d) */
        if (ch == '"' || ch == '\'' || ch == ';' || ch == ')' || ch == '(') {
            continue;
        }
        if (ch == 0xe2 && (unsigned char)token[i+1] == 0x80) {
            i += 2;
            continue;
        }
        clean[ci++] = token[i];
    }
    clean[ci] = '\0';

    /* Trim leading and trailing whitespace */
    i = 0;
    while (clean[i] && isspace((unsigned char)clean[i])) i++;
    ci = (int)strlen(clean) - 1;
    while (ci >= i && isspace((unsigned char)clean[ci])) {
        clean[ci] = '\0';
        ci--;
    }
    memmove(token, clean + i, strlen(clean + i) + 1);
}

void load_initial_data(Database *db, const char *filename) {
    FILE *fp = fopen(filename, "r");
    char line[256];
    if (!fp) return;

    while (fgets(line, sizeof(line), fp)) {
        char *tok;
        char name[MAX_STR] = "";
        char email[MAX_STR] = "";
        int job_val = 0;
        int active_val = 1;
        char birth[MAX_STR] = "";

        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;

        tok = strtok(line, ",");
        if (tok) strcpy(name, tok);
        tok = strtok(NULL, ",");
        if (tok) strcpy(email, tok);
        tok = strtok(NULL, ",");
        if (tok) job_val = atoi(tok);
        tok = strtok(NULL, ",");
        if (tok) active_val = atoi(tok);
        tok = strtok(NULL, ",");
        if (tok) strcpy(birth, tok);

        add_customer(db, name, email, int_to_job_type(job_val), active_val, birth);
    }
    fclose(fp);
}

void parse_insert_command(Database *db, const char *cmd) {
    char name[MAX_STR] = "null";
    char email[MAX_STR] = "";
    JobType job = BACKEND_DEVELOPER;
    int active = 0;
    char birth[MAX_STR] = "0.0.0";
    const char *start;
    const char *p;
    char field[MAX_STR];
    int f_idx = 0;
    int field_no = 0;

    start = strchr(cmd, '(');
    if (!start) return;
    start++;

    for (p = start; *p != '\0' && *p != ';'; p++) {
        if (*p == ',' || *p == ')') {
            field[f_idx] = '\0';
            clean_token(field);

            if (field_no == 0) {
                if (strlen(field) > 0) strcpy(name, field);
                else strcpy(name, "null");
            } else if (field_no == 1) {
                if (strlen(field) > 0) strcpy(email, field);
            } else if (field_no == 2) {
                if (strlen(field) > 0) job = int_to_job_type(atoi(field));
                else job = BACKEND_DEVELOPER;
            } else if (field_no == 3) {
                if (strlen(field) > 0) active = atoi(field);
                else active = 0;
            } else if (field_no == 4) {
                if (strlen(field) > 0) strcpy(birth, field);
                else strcpy(birth, "0.0.0");
            }

            field_no++;
            f_idx = 0;
            if (*p == ')') break;
        } else {
            if (f_idx < MAX_STR - 1) {
                field[f_idx++] = *p;
            }
        }
    }

    add_customer(db, name, email, job, active, birth);
}

void execute_commands(Database *db, const char *cmd_filename, const char *out_filename) {
    FILE *cfp = fopen(cmd_filename, "r");
    FILE *ofp = fopen(out_filename, "w");
    char line[512];

    if (!cfp || !ofp) {
        if (cfp) fclose(cfp);
        if (ofp) fclose(ofp);
        return;
    }

    /* Print initial state */
    print_table(db, ofp);

    while (fgets(line, sizeof(line), cfp)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;

        if (strstr(line, "TRUNCATE TABLE") != NULL) {
            truncate_db(db);
            print_table(db, ofp);
        } else if (strstr(line, "INSERT INTO") != NULL) {
            parse_insert_command(db, line);
            print_table(db, ofp);
        } else if (strstr(line, "DELETE FROM") != NULL) {
            char *where = strstr(line, "WHERE id=");
            if (where) {
                int del_id = atoi(where + 9);
                delete_customer(db, del_id);
                print_table(db, ofp);
            }
        } else if (strstr(line, "UPDATE CUSTOMER") != NULL) {
            char *where = strstr(line, "WHERE id=");
            if (where) {
                int upd_id = atoi(where + 9);
                char new_email[MAX_STR] = "";
                int has_job = 0;
                JobType new_job = BACKEND_DEVELOPER;
                char *em_ptr = strstr(line, "email=");
                char *job_ptr = strstr(line, "job_type=");

                if (em_ptr) {
                    char temp[MAX_STR];
                    char *comma = strchr(em_ptr, ',');
                    if (comma) {
                        int len = (int)(comma - (em_ptr + 6));
                        strncpy(temp, em_ptr + 6, len);
                        temp[len] = '\0';
                    } else {
                        strcpy(temp, em_ptr + 6);
                    }
                    clean_token(temp);
                    strcpy(new_email, temp);
                }

                if (job_ptr) {
                    int val = atoi(job_ptr + 9);
                    new_job = int_to_job_type(val);
                    has_job = 1;
                }

                if (update_customer(db, upd_id, new_email, has_job, new_job)) {
                    print_table(db, ofp);
                } else {
                    fprintf(ofp, "error\n----------\n");
                }
            }
        }
    }

    fclose(cfp);
    fclose(ofp);
}

int main(void) {
    Database db;
    init_db(&db);
    load_initial_data(&db, "input.txt");
    execute_commands(&db, "commands.txt", "my_output.txt");
    free_db(&db);
    printf("Mini Database execution complete.\n");
    return 0;
}
