#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_CHARS 27  // Total letters Including 1 space
#define MAX_NAME_LENGTH 100 // Max file name
#define FILE_LIMIT 100  // Max number of the language files

// Custom absolute value function (helper)
double absolute_value(double num) {
    return (num < 0) ? -num : num;
}

// Function to calculate the dissimilarity score between two languages
double compute_difference(double freq1[TOTAL_CHARS][TOTAL_CHARS], double freq2[TOTAL_CHARS][TOTAL_CHARS]) {
    double difference = 0.0;
    for (int i = 0; i < TOTAL_CHARS; i++) {
        for (int j = 0; j < TOTAL_CHARS; j++) {
            difference += absolute_value(freq1[i][j] - freq2[i][j]);
        }
    }
    return difference;
}

// Function to calculate frequencies from bigram counts
void compute_frequencies(double pair_freq[TOTAL_CHARS][TOTAL_CHARS], double pair_total, double frequencies[TOTAL_CHARS][TOTAL_CHARS]) {
    for (int i = 0; i < TOTAL_CHARS; i++) {
        for (int j = 0; j < TOTAL_CHARS; j++) {
            frequencies[i][j] = (pair_total > 0) ? pair_freq[i][j] / pair_total : 0.0;
        }
    }
}

// Function to calculate bigram counts and total pairs from a file
int compute_pair_frequencies(const char *filename, double pair_freq[TOTAL_CHARS][TOTAL_CHARS], double *pair_total) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    // Initialize the bigram frequency array
    for (int i = 0; i < TOTAL_CHARS; i++) {
        for (int j = 0; j < TOTAL_CHARS; j++) {
            pair_freq[i][j] = 0;
        }
    }
    *pair_total = 0;

    char prev_char = '\0';  // Holds the previous character
    char curr_char;

    // Read characters from the file to calculate bigram counts
    while ((curr_char = fgetc(file)) != EOF) {
        // Skip invalid characters and consecutive spaces
        if (curr_char == '\r' || curr_char == '\n' || (prev_char == ' ' && curr_char == ' ')) {
            continue;
        }

        // Determine the indices for the current and previous characters
        int prev_idx = (prev_char >= 'a' && prev_char <= 'z') ? prev_char - 'a' : (prev_char == ' ' ? 26 : -1);
        int curr_idx = (curr_char >= 'a' && curr_char <= 'z') ? curr_char - 'a' : (curr_char == ' ' ? 26 : -1);

        // Update the bigram counts if the indices are valid
        if (prev_idx != -1 && curr_idx != -1) {
            pair_freq[prev_idx][curr_idx]++;
            (*pair_total)++;
        }

        prev_char = curr_char;  // Update previous character
    }

    fclose(file);
    return 0;
}

int main() {
    char reference_file[MAX_NAME_LENGTH] = "language_x.txt";  // Reference language file
    double ref_pair_freq[TOTAL_CHARS][TOTAL_CHARS];
    double ref_total_pairs;
    double ref_frequencies[TOTAL_CHARS][TOTAL_CHARS];

    // Calculate bigram counts and frequencies for the reference file
    if (compute_pair_frequencies(reference_file, ref_pair_freq, &ref_total_pairs) == -1) {
        return 1;  // Exit if the reference file cannot be opened
    }
    compute_frequencies(ref_pair_freq, ref_total_pairs, ref_frequencies);

    // Process each comparison file (language_1.txt to language_FILE_LIMIT.txt)
    for (int i = 1; i <= FILE_LIMIT; i++) {
        char comp_file[MAX_NAME_LENGTH];
        sprintf(comp_file, "language_%d.txt", i);

        double comp_pair_freq[TOTAL_CHARS][TOTAL_CHARS];
        double comp_total_pairs;
        double comp_frequencies[TOTAL_CHARS][TOTAL_CHARS];

        // Calculate bigram counts and frequencies for the comparison file
        if (compute_pair_frequencies(comp_file, comp_pair_freq, &comp_total_pairs) == -1) {
            continue;  // Skip this file if it can't opened
        }
        compute_frequencies(comp_pair_freq, comp_total_pairs, comp_frequencies);

        // Calculate and print the dissimilarity score
        double dissimilarity = compute_difference(ref_frequencies, comp_frequencies);
        printf("%.6lf\n", dissimilarity);
    }

    return 0;
}
