#include <stdio.h>
#include <math.h>

// Function to enter and save rocket parameters to a file
void enter_rocket_parameters() {
    FILE *dosya = fopen("rocket_data.txt", "w");
    if (dosya == NULL) {
        printf("Error opening file.\n");
        return;
    }

    double velocity, gravity, launch_height;

    // Get parameters from the user
    printf("Enter initial velocity (m/s): ");
    scanf("%lf", &velocity);

    printf("Enter gravity (m/s², default 9.8): ");
    scanf("%lf", &gravity);

    printf("Enter launch height (m): ");
    scanf("%lf", &launch_height);

    // Save parameters to file
    fprintf(dosya, "%lf %lf %lf", velocity, gravity, launch_height);
    fclose(dosya);

    printf("Rocket parameters saved to rocket_data.txt!\n");
}

// Function to draw ASCII graph of rocket trajectory
void draw_graph(double velocity, double gravity, double launch_height) {
    printf("Reading rocket parameters from file...\n");
    printf("Equation: h(t) = -%.2lf * t^2 + %.2lf * t + %.2lf\n", 0.5 * gravity, velocity, launch_height);
    printf("Simulating trajectory...\n");

    // Calculate maximum height and time to peak
    double hmax = (velocity * velocity) / (2 * gravity) + launch_height;
    double t_peak = velocity / gravity;

    // Loop through height levels and print graph line by line
    for (int y = (int)(hmax / 10) * 10 + 10; y >= 0; y -= 10) {
        printf("%3d |", y);

        double t = 0.0;
        while (t <= t_peak) {
            double h = -0.5 * gravity * t * t + velocity * t + launch_height;

            if ((int)h >= y && (int)h < y + 10) {
                printf("  #");
            } else {
                printf("   ");
            }
            t += 1.0;
        }
        printf("\n");
    }

    // Print time axis
    printf("    +");
    for (double t = 0.0; t <= t_peak; t += 1.0) {
        printf("---");
    }
    printf("\n    ");
    for (int t = 0; t <= (int)t_peak; t += 1) {
        if (t % 2 == 0) printf("%2d ", t);
        else printf("   ");
    }
    printf("\n");
}

// Function to read parameters from file and draw the graph
void simulate_and_graph_trajectory() {
    FILE *dosya = fopen("rocket_data.txt", "r");
    if (dosya == NULL) {
        printf("Please enter parameters first.\n");
        return;
    }

    double velocity, gravity, launch_height;
    fscanf(dosya, "%lf %lf %lf", &velocity, &gravity, &launch_height);
    fclose(dosya);

    draw_graph(velocity, gravity, launch_height);
}

// Function to save trajectory graph and details to a file
void save_trajectory_to_file(double velocity, double gravity, double launch_height) {
    FILE *file = fopen("trajectory.txt", "w");

    // Calculate trajectory info
    double hmax = (velocity * velocity) / (2 * gravity) + launch_height;
    double t_peak = velocity / gravity;
    double t_flight = (velocity + sqrt(velocity * velocity + 2 * gravity * launch_height)) / gravity;

    // Write flight data to file
    fprintf(file, "Maximum Altitude: %.2lf meters\n", hmax);
    fprintf(file, "Total Flight Duration: %.2lf seconds\n\n", t_flight);

    // Save graph to file
    for (int y = (int)(hmax / 10) * 10 + 10; y >= 0; y -= 10) {
        fprintf(file, "%3d |", y);

        for (double t = 0.0; t <= t_peak; t += 1.0) {
            double h = -0.5 * gravity * t * t + velocity * t + launch_height;

            if ((int)h >= y && (int)h < y + 10) {
                fprintf(file, "  #");
            } else {
                fprintf(file, "   ");
            }
        }
        fprintf(file, "\n");
    }

    // Write time axis
    fprintf(file, "    +");
    for (double t = 0.0; t <= t_peak; t += 1.0) {
        fprintf(file, "---");
    }
    fprintf(file, "\n    ");
    for (int t = 0; t <= (int)t_peak; t += 1) {
        if (t % 2 == 0)
            fprintf(file, "%2d ", t);
        else
            fprintf(file, "   ");
    }
    fprintf(file, "\n");

    fclose(file);

    // Print confirmation to console
    printf("Saving trajectory data...\n\n");
    printf("Maximum Altitude: %.2lf meters\n", hmax);
    printf("Total Flight Duration: %.2lf seconds\n", t_flight);
    printf("Graph saved to trajectory.txt!\n");
}

int main() {
    int option;

    while (1) {
        // Display main menu
        printf("\n1. Enter rocket launch parameters\n");
        printf("2. Simulate and graph the rocket's trajectory\n");
        printf("3. Display and save the trajectory data\n");
        printf("4. Exit the program\n");

        printf("Select an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                enter_rocket_parameters();
                break;
            case 2:
                simulate_and_graph_trajectory();
                break;
            case 3: {
                FILE *dosya = fopen("rocket_data.txt", "r");
                if (dosya == NULL) {
                    printf("Read parameters first.\n");
                    break;
                }
                double velocity, gravity, launch_height;
                fscanf(dosya, "%lf %lf %lf", &velocity, &gravity, &launch_height);
                fclose(dosya);

                save_trajectory_to_file(velocity, gravity, launch_height);
                break;
            }
            case 4:
                return 0;
            default:
                printf("Invalid option. Please select between 1 and 4.\n");
        }
    }

    return 0;
}
