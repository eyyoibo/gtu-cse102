#include <stdio.h>
#define PI 3.14

int main()
{
 float diameter, radius, area;
    printf("r:");
    scanf("%f", &radius);
    area = 4 * PI * radius * radius;
    printf("Area of sphere: %0.2f", area);

    return 0;
}