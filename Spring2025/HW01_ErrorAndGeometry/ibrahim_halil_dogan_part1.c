#include <stdio.h>

int main() {
    float num1, num2, expected, experimental, error, error_percentage;
    float total_error_percentage = 0;
    char op;

    // For operation 1
    printf("Enter an operation (Example: 3+2): ");
    scanf("%f %c %f", &num1, &op, &num2);
    if (op == '+') { expected = num1 + num2; experimental = num1 - num2; }
    if (op == '-') { expected = num1 - num2; experimental = num1 * num2; }
    if (op == '*') { expected = num1 * num2; experimental = num1 + num2; }
    if (op == '/') { expected = num1 / num2; experimental = num1 - num2; }
    error = (expected > experimental) ? (expected - experimental) : (experimental - expected);
    if (expected != 0) error_percentage = (error / expected) * 100;
    else error_percentage = 0;
    total_error_percentage += error_percentage;
    printf("Expected Result: %0.2f\nExperimental Result: %0.2f\nError: %0.2f\nError Percentage: %0.2f\n", expected, experimental, error, error_percentage);

    // For operation 2
    printf("Enter an operation (Example: 3+2): ");
    scanf("%f %c %f", &num1, &op, &num2);
    if (op == '+') { expected = num1 + num2; experimental = num1 - num2; }
    if (op == '-') { expected = num1 - num2; experimental = num1 * num2; }
    if (op == '*') { expected = num1 * num2; experimental = num1 + num2; }
    if (op == '/') { expected = num1 / num2; experimental = num1 - num2; }
    error = (expected > experimental) ? (expected - experimental) : (experimental - expected);
    if (expected != 0) error_percentage = (error / expected) * 100;
    else error_percentage = 0;
    total_error_percentage += error_percentage;
    printf("Expected Result: %0.2f\nExperimental Result: %0.2f\nError: %0.2f\nError Percentage: %0.2f\n", expected, experimental, error, error_percentage);
    
    // For operation 3
    printf("Enter an operation (Example: 3+2): ");
    scanf("%f %c %f", &num1, &op, &num2);
    if (op == '+') { expected = num1 + num2; experimental = num1 - num2; }
    if (op == '-') { expected = num1 - num2; experimental = num1 * num2; }
    if (op == '*') { expected = num1 * num2; experimental = num1 + num2; }
    if (op == '/') { expected = num1 / num2; experimental = num1 - num2; }
    error = (expected > experimental) ? (expected - experimental) : (experimental - expected);
    if (expected != 0) error_percentage = (error / expected) * 100;
    else error_percentage = 0;
    total_error_percentage += error_percentage;
    printf("Expected Result: %0.2f\nExperimental Result: %0.2f\nError: %0.2f\nError Percentage: %0.2f\n", expected, experimental, error, error_percentage);
    
    // For operation 4
    printf("Enter an operation (Example: 3+2): ");
    scanf("%f %c %f", &num1, &op, &num2);
    if (op == '+') { expected = num1 + num2; experimental = num1 - num2; }
    if (op == '-') { expected = num1 - num2; experimental = num1 * num2; }
    if (op == '*') { expected = num1 * num2; experimental = num1 + num2; }
    if (op == '/') { expected = num1 / num2; experimental = num1 - num2; }
    error = (expected > experimental) ? (expected - experimental) : (experimental - expected);
    if (expected != 0) error_percentage = (error / expected) * 100;
    else error_percentage = 0;
    total_error_percentage += error_percentage;
    printf("Expected Result: %0.2f\nExperimental Result: %0.2f\nError: %0.2f\nError Percentage: %0.2f\n", expected, experimental, error, error_percentage);
    
    // Average Error
    printf("Average Error Value: %0.2f\n", total_error_percentage / 4);
    
    return 0;
}
