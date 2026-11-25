#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int stack_var = 10;
    int *heap_array = calloc(3, sizeof *heap_array);
    if (!heap_array) return EXIT_FAILURE;
    printf("Stack variable: %p\n", (void *)&stack_var);
    printf("Stack pointer: %p\n", (void *)&heap_array);
    printf("Heap (3): %p\n", (void *)heap_array);
    for (int i = 0; i < 3; i++) heap_array[i] = (i + 1) * 10;
    int *tmp = realloc(heap_array, 5 * sizeof *heap_array);
    if (!tmp) { free(heap_array); return EXIT_FAILURE; }
    heap_array = tmp;
    printf("Heap (5): %p\n", (void *)heap_array);
    for (int i = 3; i < 5; i++) heap_array[i] = (i + 1) * 10;
    for (int i = 0; i < 5; i++)
        printf("[%d] = %d, address = %p\n", i, heap_array[i], (void *)(heap_array+i));
    free(heap_array);
    heap_array = NULL;
    puts("Memory freed");
    return 0;
}
