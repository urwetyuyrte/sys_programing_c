#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void Calculations(double *a, double *b, double *c) {
    double sum = *a + *b + *c;
    double product = *a * *b * *c;
    double trig = ((int)*b % 2 == 0) ? sin(sum) : cos(product);
    *a = sum;
    *b = product;
    *c = trig;
}

int sum_of_digits(int n) {
    if (n == 0) return 0;
    return abs(n % 10) + sum_of_digits(n / 10);
}

int main(void) {
    double group = 80, variant = 22, course = 4;
    const int g = (int)group, v = (int)variant, k = (int)course;
    printf("Task 1: %.0f %.0f %.0f\n", group, variant, course);
    Calculations(&group, &variant, &course);
    printf("Sum = %.0f; Product = %.0f; Sin = %.6f\n", group, variant, course);

    int number;
    printf("Task 2: Enter integer: ");
    if (scanf("%d", &number) != 1) return 1;
    printf("Digit sum = %d\n", sum_of_digits(number));

    double values[] = {1, 2, group, variant, course};
    printf("Task 3: Initial: ");
    for (int i = 0; i < 5; ++i) printf("%.6f ", values[i]);
    values[0] = g;
    values[1] = k;
    printf("\nUpdated: ");
    for (int i = 0; i < 5; ++i) {
        values[i] += v;
        printf("%.6f ", values[i]);
    }
    printf("\nTask 4: Enter array size: ");
    int n;
    if (scanf("%d", &n) != 1 || n < 1 || n > 10000) return 1;
    int *arr = malloc((size_t)n * sizeof *arr);
    if (!arr) return 1;
    srand((unsigned)time(NULL));
    int *min_ptr = arr, *max_ptr = arr;
    puts("Initial array:");
    for (int *p = arr; p < arr + n; ++p) {
        *p = rand() % 100;
        printf("%td: %d | %p\n", p-arr, *p, (void *)p);
        if (*p < *min_ptr) min_ptr = p;
        if (*p > *max_ptr) max_ptr = p;
    }
    printf("Min: %d | %p\n", *min_ptr, (void *)min_ptr);
    printf("Max: %d | %p\n", *max_ptr, (void *)max_ptr);
    int temp = *min_ptr;
    *min_ptr = *max_ptr;
    *max_ptr = temp;
    puts("Array after swap:");
    for (int *p = arr; p < arr + n; ++p)
        printf("%td: %d | %p\n", p-arr, *p, (void *)p);
    free(arr);
    return 0;
}
