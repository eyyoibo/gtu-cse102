#include <stdio.h>

// Function to calculate the average of an array
float calculate_average(int arr[], int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

// Function to calculate the mode within a specified range [a, b]
void calculate_mode(int arr[], int n, int a, int b) {
    // Calculate mode within the interval [a, b]
    int freq[100] = {0};  // Assuming elements are within 0-99 for simplicity
    int max_freq = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] >= a && arr[i] <= b) {
            freq[arr[i]]++;
            if (freq[arr[i]] > max_freq) {
                max_freq = freq[arr[i]];
            }
        }
    }

    // Print numbers with the highest frequency (mode)
    int first_mode_printed = 0;
    for (int i = 0; i < 100; i++) {
        if (freq[i] == max_freq && freq[i] > 0) {
            if (first_mode_printed) {
                printf(" ");
            }
            printf("%d", i);
            first_mode_printed = 1;
        }
    }
    printf("\n");
}

// Function to calculate the median of an array without using a temporary variable
float calculate_median(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                arr[j] = arr[j] + arr[j + 1];
                arr[j + 1] = arr[j] - arr[j + 1];
                arr[j] = arr[j] - arr[j + 1];
            }
        }
    }

    if (n % 2 != 0) {
        return arr[n / 2];
    } else {
        return (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    }
}

int main() {
    int number_array[100] = {3, 8, 9, 9, 5, 2, 4, 7, 4, 8, 9, 10, 2, 4, 5, -1};
    int n = 16; // there is 16 value in the beggining.

    int a, b;
    printf("Enter A and B values: ");
    scanf("%d %d", &a, &b);

    float average = calculate_average(number_array, n);
    printf("Average: %.2f\n", average);

    printf("Mode: ");
    calculate_mode(number_array, n, a, b);

    float median = calculate_median(number_array, n);
    printf("Median: %.2f\n", median);

    int add_more = 1;
    printf("Would you like to add new numbers? (Press 1 for yes, Press 0 for no): ");
    scanf("%d", &add_more);

    if (add_more == 1) {
        int new_number;
        printf("Enter a new number: ");
        scanf("%d", &new_number);

        if (new_number != -1) {
            number_array[n] = new_number;
            n++;

            average = calculate_average(number_array, n);
            printf("Average: %.2f\n", average);

            printf("Mode: ");
            calculate_mode(number_array, n, a, b);

            median = calculate_median(number_array, n);
            printf("Median: %.2f\n", median);
        }
    }

    return 0;
}
