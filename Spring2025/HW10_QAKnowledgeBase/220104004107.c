#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PAIRS 200
#define MAX_EMB_LEN 24120
#define THRESHOLD 0.8

// Structure to hold QA embeddings
typedef struct {
    int question[MAX_EMB_LEN];
    int answer[MAX_EMB_LEN];
} QAPair;

QAPair qaPairs[MAX_PAIRS];
int knowledgeBase[MAX_PAIRS][MAX_EMB_LEN];
int numPairs = 0;
int embLength = 0;

// Function to parse a binary string into integer array
void parse_embedding(char *line, int *embedding) {
    for (int i = 0; i < embLength; ++i) {
        embedding[i] = line[i] - '0';
    }
}

// XOR operation between two embeddings
void xor_embedding(int *a, int *b, int *result) {
    for (int i = 0; i < embLength; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

// XOR and sum for matching score
double matching_score(int *a, int *b) {
    int sum = 0;
    for (int i = 0; i < embLength; ++i) {
        sum += a[i] ^ b[i];
    }
    return 1.0 - ((double)sum / embLength);
}

// Load embeddings from file
void load_embeddings(const char *filename) {
    FILE *file = fopen(filename, "r");
    char line[50000];
    int mode = 0; // 0 = skip, 1 = read question, 2 = read answer

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Maximum Sentence Embedding Length", 33) == 0) {
            sscanf(line, "Maximum Sentence Embedding Length: %d", &embLength);
        } else if (strncmp(line, "Question:", 8) == 0) {
            parse_embedding(line + 9, qaPairs[numPairs].question);
            mode = 1;
        } else if (strncmp(line, "Answer:", 7) == 0) {
            parse_embedding(line + 8, qaPairs[numPairs].answer);
            ++numPairs;
            mode = 0;
        }
    }
    fclose(file);
}

// Build knowledge base with XORed Q^A embeddings
void build_knowledge_base() {
    for (int i = 0; i < numPairs; ++i) {
        xor_embedding(qaPairs[i].question, qaPairs[i].answer, knowledgeBase[i]);
    }
}

// Load answer texts from database.txt
void load_answers(char answers[][1024]) {
    FILE *file = fopen("database.txt", "r");
    char line[1024];
    int index = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "A:", 2) == 0) {
            strcpy(answers[index++], line + 3);
        }
    }
    fclose(file);
}

int main() {
    load_embeddings("embeddings.txt");
    build_knowledge_base();

    // Ask user for input embedding
    char input[50000];
    int query[MAX_EMB_LEN];
    int tempAns[MAX_EMB_LEN];
    char answers[MAX_PAIRS][1024];

    load_answers(answers);

    printf("Enter a question embedding (as binary string):\n");
    scanf("%s", input);
    parse_embedding(input, query);

    // For each KB entry, calculate answer = KB XOR Q
    for (int i = 0; i < numPairs; ++i) {
        xor_embedding(knowledgeBase[i], query, tempAns);

        // Compare to each known answer
        for (int j = 0; j < numPairs; ++j) {
            double score = matching_score(tempAns, qaPairs[j].answer);
            if (score >= THRESHOLD) {
                printf("Matched answer with %.2f%% match: %s", score * 100, answers[j]);
            }
        }
    }

    return 0;
}
