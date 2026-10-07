#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GROUPS 128

// Function prototypes
void readPPMFile(char *filename, int ***grid, int *rows, int *cols);
void writePPMFile(char *filename, int **grid, int rows, int cols);
void findGroups(int **grid, int **outputGrid, int rows, int cols);
void exploreGroup(int **grid, int **outputGrid, int rows, int cols, int x, int y, int groupNumber);
void freeGrid(int **grid, int rows);

int main() {
	int **grid = NULL;
	int **outputGrid = NULL;
	int rows, cols;

	// Read input grid from PPM file
	readPPMFile("input.ppm", &grid, &rows, &cols);

	// Allocate memory for output grid
	outputGrid = (int **)malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++) {
		outputGrid[i] = (int *)calloc(cols, sizeof(int)); // Initialize the grid to zero
	}

	// Find and label groups in the grid
	findGroups(grid, outputGrid, rows, cols);

	// Write labeled grid to PPM file
	writePPMFile("output.ppm", outputGrid, rows, cols);

	// Free allocated memory
	freeGrid(grid, rows);
	freeGrid(outputGrid, rows);

	return 0;
}

// Function to read a PPM file and load the grid data
void readPPMFile( char *filename, int ***grid, int *rows, int *cols) {
	FILE *file = fopen(filename, "r");

	char format[3];
	fscanf(file, "%2s", format);  // Read the format (should be P1)

	fscanf(file, "%d %d", cols, rows); // Read grid dimensions

	// Allocate memory for the grid
	*grid = (int **)malloc(*rows * sizeof(int *));
	for (int i = 0; i < *rows; i++) {
		(*grid)[i] = (int *)malloc(*cols * sizeof(int));
		for (int j = 0; j < *cols; j++) {
			fscanf(file, "%d", &(*grid)[i][j]); // Read each pixel value (0 or 1)
		}
	}

	fclose(file);
}

// Function to write a grid to a PPM file with group colors
void writePPMFile(char *filename, int **grid, int rows, int cols) {
	FILE *file = fopen(filename, "w");

	// Define a scaling factor for the output image
	int scale = 1;

	// Write the PPM header (P3 format with scaled dimensions)
	fprintf(file, "P3\n%d %d\n255\n", cols * scale, rows * scale);

	// Generate random colors for each group
	int colors[MAX_GROUPS][3];
	for (int i = 1; i <= MAX_GROUPS; i++) {
		colors[i - 1][0] = rand() % 256; // Red
		colors[i - 1][1] = rand() % 256; // Green
		colors[i - 1][2] = rand() % 256; // Blue
	}

	// Write the grid with scaled colors
	for (int i = 0; i < rows; i++) {
		for (int si = 0; si < scale; si++) { // Scale each row
			for (int j = 0; j < cols; j++) {
				for (int sj = 0; sj < scale; sj++) { // Scale each column
					int group = grid[i][j];
					if (group == 0) {
						fprintf(file, "0 0 0 "); // Black for group 0 (background)
					} else {
						// Write the color of the group
						fprintf(file, "%d %d %d ", colors[group - 1][0], colors[group - 1][1], colors[group - 1][2]);
					}
				}
			}
			fprintf(file, "\n"); // New line after scaling the row
		}
	}

	fclose(file);
}

// Function to find and label groups in the grid
void findGroups(int **grid, int **outputGrid, int rows, int cols) {
	int groupNumber = 1;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (grid[i][j] == 1 && outputGrid[i][j] == 0) {
				// Start a new group if the cell is part of a group and not yet labeled
				exploreGroup(grid, outputGrid, rows, cols, i, j, groupNumber);
				groupNumber++; // Increment group number
				if (groupNumber > MAX_GROUPS) {
					printf("ERROR: Maximum number of groups exceeded\n");
					break; // Exit the loop instead of exiting the program
				}

			}
		}
	}
}

// Function to explore and label the connected cells of a group
void exploreGroup(int **grid, int **outputGrid, int rows, int cols, int x, int y, int groupNumber) {
	if (x < 0 || x >= rows || y < 0 || y >= cols || grid[x][y] == 0 || outputGrid[x][y] != 0) {
		return; // Return if out of bounds, empty, or already labeled
	}

	outputGrid[x][y] = groupNumber; // Label the cell with the current group number

	// Recursively explore neighboring cells (up, down, left, right)
	exploreGroup(grid, outputGrid, rows, cols, x - 1, y, groupNumber); // Up
	exploreGroup(grid, outputGrid, rows, cols, x + 1, y, groupNumber); // Down
	exploreGroup(grid, outputGrid, rows, cols, x, y - 1, groupNumber); // Left
	exploreGroup(grid, outputGrid, rows, cols, x, y + 1, groupNumber); // Right
}

// Function to free the allocated memory for a grid
void freeGrid(int **grid, int rows) {
	for (int i = 0; i < rows; i++) {
		free(grid[i]); // Free each row
	}
	free(grid); // Free the grid itself
}