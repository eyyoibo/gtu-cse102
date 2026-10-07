#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 31 // Maximum length of module names
#define MAX_TERMS_COUNT 100 // Maximum number of terms in the description

// Checks if a character is a digit.
int is_digit_char(char character) {
    return character >= '0' && character <= '9';
}

// Checks if a character is a valid module character (letter or '_').
int is_valid_module_character(char character) {
    return (character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == '_';
}

// Analyzes a given module expression and calculates the result.
int compute_module_result(const char *module_name) {
    char input[256];
    printf("Define %s:\n", module_name);
    fgets(input, sizeof(input), stdin);

    int total_sum = 0, scalar_value = 0, module_index = 0;
    char current_module[MAX_NAME_LENGTH];
    int processing_scalar = 1; // Flag to check if we are processing a scalar value.
    int term_count = 0; // Counter to ensure no more than 100 terms.

    // Iterate through each character of the input.
    for (int i = 0; i < strlen(input); i++) {
        char current_char = input[i];

        if (term_count >= MAX_TERMS_COUNT) {
            printf("Maximum number of terms exceeded.\n");
            break; // Exit if there are more than 100 terms.
        }

        if (is_digit_char(current_char) && processing_scalar) {
            // If processing a scalar, concatenate the digits.
            scalar_value = scalar_value * 10 + (current_char - '0');
        } else if (current_char == '*') {
            // '*' character indicates the end of the scalar part.
            processing_scalar = 0; // Scalar processing is done, moving to module name.
        } else if (is_valid_module_character(current_char)) {
            // If a valid module character, build the module name.
            current_module[module_index++] = current_char;
        } else if (current_char == '+' || current_char == '\n') {
            // When a term is completed:
            if (module_index > 0) { // If there is a module name:
                current_module[module_index] = '\0'; // Terminate the module name.
                total_sum += scalar_value * compute_module_result(current_module); // Evaluate the sub-module.
            } else {
                // If only scalar, add directly to the sum.
                total_sum += scalar_value;
            }
            scalar_value = 0; // Reset scalar.
            module_index = 0;  // Reset module name.
            processing_scalar = 1; // Next term starts with a scalar.
            term_count++; // Increment term count.
        }
    }

    // Process the last term if it exists.
    if (module_index > 0) {
        current_module[module_index] = '\0';
        total_sum += scalar_value * compute_module_result(current_module);
    } else {
        total_sum += scalar_value;
    }

    return total_sum; // Return the calculated total.
}

int main() {
    char module_name[MAX_NAME_LENGTH];
    printf("Module name?:\n");
    fgets(module_name, sizeof(module_name), stdin);
    module_name[strcspn(module_name, "\n")] = '\0'; // Clean the newline.

    int result = compute_module_result(module_name); // Calculate the result of the module.
    printf("%d\n", result); // Print the result.

    return 0;
}
