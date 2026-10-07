#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns the maximum product of elements in two equal-length arrays
int max_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int max = chunk[0] * corresponding_chunk_part[0];
    for (int i = 1; i < chunk_size; i++) {
        int product = chunk[i] * corresponding_chunk_part[i];
        if (product > max) {
            max = product;
        }
    }
    return max;
}

// Returns the average of the product of elements in two equal-length arrays
int avg_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        sum += chunk[i] * corresponding_chunk_part[i];
    }
    return sum / chunk_size;
}

// Reads a line from a file and parses comma-separated integers into an array
int* read_line(FILE* file, int* length) {
    char buffer[300];
    if (file == NULL || fgets(buffer, sizeof(buffer), file) == NULL) {
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    int* arr = NULL;
    int count = 0;
    char* token = strtok(buffer, ",");
    while (token != NULL) {
        arr = realloc(arr, (count + 1) * sizeof(int));
        arr[count] = atoi(token);
        count++;
        token = strtok(NULL, ",");
    }
    *length = count;
    return arr;
}

// Processes the input line with sliding chunks and selected operation
int* process_line(int* line, int line_length, int* multipliers, int chunk_size, char operation) {
    int num_chunks = line_length - chunk_size + 1;
    int* results = malloc(num_chunks * sizeof(int));
    for (int i = 0; i < num_chunks; i++) {
        int* chunk = line + i;
        if (operation == 'm') {
            results[i] = max_operation(chunk, multipliers, chunk_size);
        } else {
            results[i] = avg_operation(chunk, multipliers, chunk_size);
        }
    }
    return results;
}

// Writes the results to output file in the selected color mode
void print_output_file(FILE* output, char* color, int* line2_results, int* line3_results, int* line4_results, int num_results) {
    if (strcmp(color, "rgb") == 0) {
        fprintf(output, "RGB : -> ");
        for (int i = 0; i < num_results; i++) {
            fprintf(output, "[%d,%d,%d]", line2_results[i], line3_results[i], line4_results[i]);
            if (i < num_results - 1) fprintf(output, ",");
        }
    } else if (strcmp(color, "bgr") == 0) {
        fprintf(output, "BGR : -> ");
        for (int i = 0; i < num_results; i++) {
            fprintf(output, "[%d,%d,%d]", line4_results[i], line3_results[i], line2_results[i]);
            if (i < num_results - 1) fprintf(output, ",");
        }
    } else if (strcmp(color, "gray") == 0) {
    fprintf(output, "GRAY : -> [");
    for (int i = 0; i < num_results; i++) {
        fprintf(output, "%d", line2_results[i]);
        if (i < num_results - 1) fprintf(output, ",");
    }
    fprintf(output, "]\n");

    // Aligned second line
    fprintf(output, "          ["); 
    for (int i = 0; i < num_results; i++) {
        fprintf(output, "%d", line3_results[i]);
        if (i < num_results - 1) fprintf(output, ",");
    }
    fprintf(output, "]\n");

    // Aligned third line
    fprintf(output, "          ["); 
    for (int i = 0; i < num_results; i++) {
        fprintf(output, "%d", line4_results[i]);
        if (i < num_results - 1) fprintf(output, ",");
    }
    fprintf(output, "]\n");
}

    else {
        fprintf(stderr, "Invalid color mode.\n");
    }
}

int main() {
    FILE* input = fopen("input.txt", "r");
    if (input == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    int first_line_length;
    int* first_line = read_line(input, &first_line_length);
    if (first_line_length != 3 && first_line_length != 6 && first_line_length != 9) {
        fprintf(stderr, "Invalid first line length. Must be 3, 6, or 9.\n");
        fclose(input);
        free(first_line);
        return 1;
    }
    int chunk_size = first_line_length;

    int line2_length, line3_length, line4_length;
    int* line2 = read_line(input, &line2_length);
    int* line3 = read_line(input, &line3_length);
    int* line4 = read_line(input, &line4_length);
    fclose(input);

    if (line2 == NULL || line3 == NULL || line4 == NULL) {
        fprintf(stderr, "Error reading lines 2-4.\n");
        free(first_line); free(line2); free(line3); free(line4);
        return 1;
    }

    if (line2_length != line3_length || line3_length != line4_length) {
        fprintf(stderr, "Lines 2-4 must have the same length.\n");
        free(first_line); free(line2); free(line3); free(line4);
        return 1;
    }

    char ops[3];
    for (int i = 0; i < 3; i++) {
        char op[4];
        printf("Enter operation %d (max/avg): ", i + 1);
        scanf("%3s", op);
        if (strcmp(op, "max") == 0) {
            ops[i] = 'm';
        } else if (strcmp(op, "avg") == 0) {
            ops[i] = 'a';
        } else {
            fprintf(stderr, "Invalid operation.\n");
            free(first_line); free(line2); free(line3); free(line4);
            return 1;
        }
    }

    char color[5];
    printf("Enter color mode (rgb/bgr/gray): ");
    scanf("%4s", color);

    int* line2_results = process_line(line2, line2_length, first_line, chunk_size, ops[0]);
    int* line3_results = process_line(line3, line3_length, first_line, chunk_size, ops[1]);
    int* line4_results = process_line(line4, line4_length, first_line, chunk_size, ops[2]);
    int num_results = line2_length - chunk_size + 1;

    FILE* output = fopen("output.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        free(first_line); free(line2); free(line3); free(line4);
        free(line2_results); free(line3_results); free(line4_results);
        return 1;
    }

    print_output_file(output, color, line2_results, line3_results, line4_results, num_results);

    fclose(output);
    free(first_line); free(line2); free(line3); free(line4);
    free(line2_results); free(line3_results); free(line4_results);

    printf("Output written to output.txt\n");
    return 0;
}
