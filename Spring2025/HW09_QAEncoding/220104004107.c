#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENTENCE_LENGTH 1000
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100
#define MAX_PAIRS 200
#define SEPARATOR "---"
#define QUESTION_PREFIX "Q:"
#define ANSWER_PREFIX "A:"

// Extract all unique characters from questions and answers
int extractUniqueChars(char questions[][MAX_SENTENCE_LENGTH], char answers[][MAX_SENTENCE_LENGTH], int numPairs, char allChars[]) {
    int charCount = 0;
    for (int i = 0; i < numPairs; i++) {
        char *text[] = {questions[i], answers[i]};
        for (int t = 0; t < 2; t++) {
            for (int j = 0; text[t][j]; j++) {
                if (!strchr(allChars, text[t][j])) {
                    allChars[charCount++] = text[t][j];
                }
            }
        }
    }
    return charCount;
}

// Find the maximum word length in a list of sentences
int findMaxWordLength(char sentences[][MAX_SENTENCE_LENGTH], int count) {
    int maxLen = 0;
    for (int i = 0; i < count; i++) {
        char temp[MAX_SENTENCE_LENGTH];
        strcpy(temp, sentences[i]);
        char *token = strtok(temp, " ");
        while (token) {
            int len = strlen(token);
            if (len > maxLen) maxLen = len;
            token = strtok(NULL, " ");
        }
    }
    return maxLen;
}

// Split a sentence into words (tokenization)
void tokenize(char *sentence, char words[][MAX_WORD_LENGTH], int *wordCount) {
    *wordCount = 0;
    char *token = strtok(sentence, " ");
    while (token) {
        strcpy(words[(*wordCount)++], token);
        token = strtok(NULL, " ");
    }
}

// Convert a character into one-hot encoded vector
void oneHotChar(char ch, int charCount, char allChars[], int *vec) {
    for (int i = 0; i < charCount; i++) vec[i] = 0;
    for (int i = 0; i < charCount; i++) {
        if (ch == allChars[i]) {
            vec[i] = 1;
            break;
        }
    }
}

// Encode a word into a fixed-size one-hot vector with left-padding
void encodeWord(char *word, int maxWordLen, int charCount, char allChars[], int *output) {
    int pad = maxWordLen - strlen(word);
    for (int i = 0; i < pad * charCount; i++) output[i] = 0; // Left-padding with zero-vectors

    for (int i = 0; word[i]; i++) {
        oneHotChar(word[i], charCount, allChars, &output[(i + pad) * charCount]);
    }
}

// Encode an entire sentence into a single one-hot vector by combining word vectors
void encodeSentence(char *sentence, int maxWordLen, int charCount, char allChars[], int **outVec, int *outLen) {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int wordCount;
    tokenize(sentence, words, &wordCount);

    int totalLen = wordCount * maxWordLen * charCount;
    int *vec = (int *)malloc(totalLen * sizeof(int));

    for (int i = 0; i < wordCount; i++) {
        encodeWord(words[i], maxWordLen, charCount, allChars, &vec[i * maxWordLen * charCount]);
    }

    *outVec = vec;
    *outLen = totalLen;
}

// Load questions and answers from input file
int loadData(char *filename, char questions[][MAX_SENTENCE_LENGTH], char answers[][MAX_SENTENCE_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if (!file) { perror("Failed to open file"); exit(1); }

    char line[MAX_SENTENCE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strncmp(line, QUESTION_PREFIX, strlen(QUESTION_PREFIX)) == 0) {
            strcpy(questions[count], line + strlen(QUESTION_PREFIX));
        } else if (strncmp(line, ANSWER_PREFIX, strlen(ANSWER_PREFIX)) == 0) {
            strcpy(answers[count], line + strlen(ANSWER_PREFIX));
            count++;
        }
    }
    fclose(file);
    return count;
}

// Write metadata and embeddings to output file
void writeOutput(char *filename, int pairCount, int maxLen, int dim,
                 int **qVecs, int **aVecs, int *qLens, int *aLens) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("Failed to write output file"); exit(1); }

    fprintf(f, "Maximum Sentence Embedding Length: %d\n", maxLen);
    fprintf(f, "Dimension of Word Embeddings: %d\n", dim);
    fprintf(f, "Number of Question-Answer Pairs: %d\n\n", pairCount);
    fprintf(f, "#####\n");

    for (int i = 0; i < pairCount; i++) {
        fprintf(f, "Question: ");
        for (int j = 0; j < qLens[i]; j++) fprintf(f, "%d", qVecs[i][j]);
        fprintf(f, "\n");

        fprintf(f, "Answer: ");
        for (int j = 0; j < aLens[i]; j++) fprintf(f, "%d", aVecs[i][j]);
        fprintf(f, "\n");

        if (i < pairCount - 1) fprintf(f, "%s\n", SEPARATOR);
    }
    fclose(f);
}

// Main function to process encoding pipeline
int main() {
    char questions[MAX_PAIRS][MAX_SENTENCE_LENGTH];
    char answers[MAX_PAIRS][MAX_SENTENCE_LENGTH];
    char allChars[MAX_SENTENCE_LENGTH];
    int pairCount = loadData("database.txt", questions, answers);

    int charCount = extractUniqueChars(questions, answers, pairCount, allChars);
    int maxWordLen = findMaxWordLength(questions, pairCount);

    int **qVecs = (int **)malloc(pairCount * sizeof(int *));
    int **aVecs = (int **)malloc(pairCount * sizeof(int *));
    int *qLens = (int *)malloc(pairCount * sizeof(int));
    int *aLens = (int *)malloc(pairCount * sizeof(int));

    int maxSentenceLen = 0;

    for (int i = 0; i < pairCount; i++) {
        encodeSentence(questions[i], maxWordLen, charCount, allChars, &qVecs[i], &qLens[i]);
        encodeSentence(answers[i], maxWordLen, charCount, allChars, &aVecs[i], &aLens[i]);
        if (qLens[i] > maxSentenceLen) maxSentenceLen = qLens[i];
        if (aLens[i] > maxSentenceLen) maxSentenceLen = aLens[i];
    }

    writeOutput("embeddings.txt", pairCount, maxSentenceLen, maxWordLen * charCount, qVecs, aVecs, qLens, aLens);

    // Free dynamically allocated memory
    for (int i = 0; i < pairCount; i++) {
        free(qVecs[i]);
        free(aVecs[i]);
    }
    free(qVecs); free(aVecs); free(qLens); free(aLens);

    return 0;
}
