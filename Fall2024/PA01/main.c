#include <stdio.h>
#include <math.h>

#define N 4 // Number of coordinates
#define X 10 // Percentage of Error
#define EPSILON 0.001 

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int find_closest_cluster(float x_cordinate, float y_cordinate,
                         float old_x_center_1, float old_y_center_1,
                         float old_x_center_2, float old_y_center_2,
                         float old_x_center_3, float old_y_center_3) {
    
    float distance1 = distance(x_cordinate, y_cordinate, old_x_center_1, old_y_center_1);
    float distance2 = distance(x_cordinate, y_cordinate, old_x_center_2, old_y_center_2);
    float distance3 = distance(x_cordinate, y_cordinate, old_x_center_3, old_y_center_3);

    float minimum_distance, second_min_distance;
    int cluster_number;

    if (distance1 <= distance2 && distance1 <= distance3) {
        minimum_distance = distance1;
        cluster_number = 1;
        if (distance2 < distance3) {
            second_min_distance = distance2;
        } else {
            second_min_distance = distance3;
        }
    } else if (distance2 <= distance1 && distance2 <= distance3) {
        minimum_distance = distance2;
        cluster_number = 2;
        if (distance1 < distance3) {
            second_min_distance = distance1;
        } else {
            second_min_distance = distance3;
        }
    } else {
        minimum_distance = distance3;
        cluster_number = 3;
        if (distance1 < distance2) {
            second_min_distance = distance1;
        } else {
            second_min_distance = distance2;
        }
    }

    float percentage_difference = ((second_min_distance - minimum_distance) / 
        ((second_min_distance + minimum_distance) / 2 + EPSILON)) * 100;

    if (percentage_difference < X && minimum_distance < second_min_distance) {
        printf("Coordinate (%f, %f) is discarded.\n", x_cordinate, y_cordinate);
        return -1; 
    }

    printf("Coordinate (%f, %f) is placed in Cluster %d\n", x_cordinate, y_cordinate, cluster_number);
    return cluster_number;
}

int main() {
    float x_cordinate, y_cordinate;
    float old_x_center_1, old_y_center_1; 
    float old_x_center_2, old_y_center_2; 
    float old_x_center_3, old_y_center_3; 

    printf("Enter Cluster 1:\n");
    scanf("%f %f", &old_x_center_1, &old_y_center_1);

    printf("Enter Cluster 2:\n");
    scanf("%f %f", &old_x_center_2, &old_y_center_2);

    printf("Enter Cluster 3:\n");
    scanf("%f %f", &old_x_center_3, &old_y_center_3);  

    int count_points_1 = 0; 
    int count_points_2 = 0; 
    int count_points_3 = 0;

    int count = 1;
    int cluster_number;

    while (count <= N) {
        printf("Enter %dth coordinates: ", count);
        scanf("%f %f", &x_cordinate, &y_cordinate);

        cluster_number = find_closest_cluster(x_cordinate, y_cordinate, 
            old_x_center_1, old_y_center_1,
            old_x_center_2, old_y_center_2,
            old_x_center_3, old_y_center_3);
        
        if (cluster_number == -1) {
            printf("Enter the coordinates again.\n");
            continue; 
        }

        float new_x_center, new_y_center;

        if (cluster_number == 1) {
            count_points_1++;
            new_x_center = (old_x_center_1 * count_points_1 + x_cordinate) / (count_points_1 + 1);
            new_y_center = (old_y_center_1 * count_points_1 + y_cordinate) / (count_points_1 + 1);
            old_x_center_1 = new_x_center;
            old_y_center_1 = new_y_center;
        } else if (cluster_number == 2) {
            count_points_2++;
            new_x_center = (old_x_center_2 * count_points_2 + x_cordinate) / (count_points_2 + 1);
            new_y_center = (old_y_center_2 * count_points_2 + y_cordinate) / (count_points_2 + 1);
            old_x_center_2 = new_x_center;
            old_y_center_2 = new_y_center;
        } else if (cluster_number == 3) {
            count_points_3++;
            new_x_center = (old_x_center_3 * count_points_3 + x_cordinate) / (count_points_3 + 1);
            new_y_center = (old_y_center_3 * count_points_3 + y_cordinate) / (count_points_3 + 1);
            old_x_center_3 = new_x_center;
            old_y_center_3 = new_y_center;
        }   

        count++;
    }
    
    printf("Cluster 1: %.4f %.4f with %d points\n", old_x_center_1, old_y_center_1, count_points_1 + 1);
    printf("Cluster 2: %.4f %.4f with %d points\n", old_x_center_2, old_y_center_2, count_points_2 + 1);
    printf("Cluster 3: %.4f %.4f with %d points\n", old_x_center_3, old_y_center_3, count_points_3 + 1);
    
    return 0;
}

