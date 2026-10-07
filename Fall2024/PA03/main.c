#include <stdio.h>
#include <math.h>

#define MAX_PAIR_COUNT 200  // Maximum number of x,y pairs
#define MAX_LINE_CHAR_COUNT 1000  // Maximum characters in line

void compute_polynomial_value(const char *polynomial, double x, double y, double *result);
void extract_monomial_terms(const char *monomial, double x, double y, double *term_result);

int main() {
    FILE *value_file = fopen("values.txt", "r");
    FILE *poly_file = fopen("polynomials.txt", "r");
    FILE *eval_file = fopen("evaluations.txt", "w");

    double xCoords[MAX_PAIR_COUNT], yCoords[MAX_PAIR_COUNT];
    char polynomial[MAX_LINE_CHAR_COUNT];
    int pair_count = 0;

    // Read all x,y pairs from valuestxt into arrays
    while (fscanf(value_file, "%lf %lf", &xCoords[pair_count], &yCoords[pair_count]) == 2) {
        pair_count++;
        if (pair_count >= MAX_PAIR_COUNT) break;
    }

    // Read each polynomial line by line
    while (fgets(polynomial, sizeof(polynomial), poly_file)) {
        for (int i = 0; i < pair_count; i++) {
            double result = 0.0;
            compute_polynomial_value(polynomial, xCoords[i], yCoords[i], &result);
            fprintf(eval_file, "%.2f ", result);
        }
        fprintf(eval_file, "\n");  // Move to the next line for the next polynomal
    }
    fclose(value_file);
    fclose(poly_file);
    fclose(eval_file);
    return 0;
}

void extract_monomial_terms(const char *monomial, double x, double y, double *term_result) {
    double coefficient = 1.0;
    int x_exponent = 0, y_exponent = 0;
    // Read the coefficent if there is 
    if (sscanf(monomial, "%lf", &coefficient) == 1) {
        while (*monomial != 'x' && *monomial != 'y' && *monomial != '\0') {
            monomial++;
        }
    }

    // Check if x exists in the monomial and determine its exponent
    if (*monomial == 'x') {
        monomial++;
        x_exponent = 1;
        if (*monomial == '^') {
            monomial++;
            sscanf(monomial, "%d", &x_exponent);
            while (*monomial >= '0' && *monomial <= '9') {
                monomial++;
            }
        }
    }

    // Check if y exists in the monomial and determine itsexponent
    if (*monomial == 'y') {
        monomial++;
        y_exponent = 1;
        if (*monomial == '^') {
            monomial++;
            sscanf(monomial, "%d", &y_exponent);
            while (*monomial >= '0' && *monomial <= '9') {
                monomial++;
            }
        }
    }
    // Calculate the result of the monomial
    *term_result = coefficient * pow(x, x_exponent) * pow(y, y_exponent);
}

// Evaluate polynomial for given x and y values
void compute_polynomial_value(const char *polynomial, double x, double y, double *result) {
    const char *ptr = polynomial;  // Pointer to iterate through the polynomial string
    double term_result = 0.0;
    int sign = 1;

    while (*ptr != '\0') {
        // Handle different characters in the polynomial string 
        if (*ptr == ' ' || *ptr == '\t') {
            ptr++;
        } else if (*ptr == '-') {
            sign = -1;
            ptr++;
        } else if (*ptr == '+') {
            sign = 1;
            ptr++;
        } else {
            extract_monomial_terms(ptr, x, y, &term_result);
            *result += sign * term_result;
            // Move pointer to the next term
            while (*ptr != '+' && *ptr != '-' && *ptr != '\0') {
                ptr++;
            }
        }
    }
}
