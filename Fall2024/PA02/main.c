#include <stdio.h>

#define X 2 // Segment size
#define Y 13 // Number of segments to remember

int main() {
    FILE *inputFile, *outputFile;
    
    inputFile = fopen("input.txt", "r");
    outputFile = fopen("output.txt", "w");

    int segments[Y][X] = {0}; // Maximum Y segments stored each holds x amount of data
    int counts[Y] = {0};       // Array to store the count of each segment
    int segment[X];            // Array to hold the current segment
    int num, i = 0, segmentCount = 0;

    // Read numbers from the file and create segments
    while (fscanf(inputFile, "%d", &num) != EOF) {
        segment[i] = num; // Add number to the segment array
        i++;

        // When the segment is full, process and prepare for the new segment
        if (i == X) {
            // Search for this segment among the stored segments
            int match = -1; 
            for (int j = 0; j < segmentCount; j++) {
                // Directly check if the segment matches
                if (segments[j][0] == segment[0] && segments[j][1] == segment[1]) {
                    counts[j]++;
                    match = j; // Store the index of the found segment
                    break;
                }
            }

            // If the segment is new
            if (match == -1) {
                if (segmentCount < Y) {
                    // Copy the segment if there's space
                    segments[segmentCount][0] = segment[0]; // Copy 1. element
                    segments[segmentCount][1] = segment[1]; // Copy 2. element
                    counts[segmentCount] = 1;
                    segmentCount++;
                } else {
                    // If exceeding Y remove the oldest segment and add the new one
                    for (int k = 1; k < Y; k++) {
                        for (int m = 0; m < X; m++) {
                            segments[k - 1][m] = segments[k][m]; // Shift segments
                        }
                        counts[k - 1] = counts[k];
                    }
                    segments[Y - 1][0] = segment[0]; // Add the new segment
                    segments[Y - 1][1] = segment[1]; // Add the new segment
                    counts[Y - 1] = 1;
                }
            }

            // For move one step forward
            for (int k = 0; k < X - 1; k++) {
                segment[k] = segment[k + 1];
            }
            i = X - 1; // Update i for the next element
        }
    }

    // Write the results to output.txt
    for (int j = 0; j < segmentCount; j++) {
        for (int k = 0; k < X; k++) {
            fprintf(outputFile, "%d ", segments[j][k]);
        }
        fprintf(outputFile, ": %d\n", counts[j]);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

