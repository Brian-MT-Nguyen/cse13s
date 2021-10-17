#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "aeisqr:n:p:h"
#define SEED    13371453
#define SIZE    100

typedef enum { INSERTION, HEAP, QUICK, SHELL, HELP } Sorts;

int main(int argc, char **argv) {

    Set sorts = empty_set();
    int opt = 0;
    uint32_t seed = SEED;
    uint32_t size = SIZE;
    uint32_t elements = SIZE;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            sorts = insert_set(INSERTION, sorts);
            sorts = insert_set(HEAP, sorts);
            sorts = insert_set(QUICK, sorts);
            sorts = insert_set(SHELL, sorts);
            break;
        case 'e': sorts = insert_set(HEAP, sorts); break;
        case 'i': sorts = insert_set(INSERTION, sorts); break;
        case 's': sorts = insert_set(SHELL, sorts); break;
        case 'q': sorts = insert_set(QUICK, sorts); break;
        case 'r': seed = atoi(optarg); break;
        case 'n': size = atoi(optarg); break;
        case 'p': elements = atoi(optarg); break;
        case 'h': sorts = insert_set(HELP, sorts); break;
        }
    }

    srand(seed);
    uint32_t bitmask = 0x3FFFFFFF;
    uint32_t *A = (uint32_t *) calloc(size, sizeof(uint32_t));
    uint32_t *B = (uint32_t *) calloc(size, sizeof(uint32_t));

    for (uint32_t i = 0; i < size; i++) {
        A[i] = (random() & bitmask);
    }
    for (uint32_t i = 0; i < size; i++) {
        B[i] = A[i];
    }
    Stats stats;
    stats.moves = 0;
    stats.compares = 0;

    if (elements > size) {
        elements = size;
    }

    if (member_set(HELP, sorts) || sorts == empty_set()) {
        if (sorts == empty_set()) {
            printf("Select at least one sort to perform.\n");
        }
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n\n");
        printf("USAGE\n");
        printf("   $ ./sorting [-aeisqr:n:p:h] [-n length] [-p elements] [-r seed]\n\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all sorts.\n");
        printf("  -e   Runs e pi test.\n");
        printf("  -b   Runs BBP pi test.\n");
        printf("  -m   Runs Madhava pi test.\n");
        printf("  -r   Runs Euler pi test.\n");
        printf("  -v   Runs Viete pi test.\n");
        printf("  -n   Runs Newton square root tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program synopsis and usage.\n");
    }
    if (member_set(HEAP, sorts)) {
        heap_sort(&stats, A, size);
        printf(
            "Heap Sort, %u elements, %lu moves, %lu compares", size, stats.moves, stats.compares);
        for (uint32_t index = 0; index < elements; index++) {
            if (index % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, A[index]);
        }
        printf("\n");
        reset(&stats);
        for (uint32_t array_index = 0; array_index < size; array_index++) {
            A[array_index] = B[array_index];
        }
    }
    if (member_set(SHELL, sorts)) {
        shell_sort(&stats, A, size);
        printf(
            "Shell Sort, %u elements, %lu moves, %lu compares", size, stats.moves, stats.compares);
        for (uint32_t index = 0; index < elements; index++) {
            if (index % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, A[index]);
        }
        printf("\n");
        reset(&stats);
        for (uint32_t array_index = 0; array_index < size; array_index++) {
            A[array_index] = B[array_index];
        }
    }
    if (member_set(INSERTION, sorts)) {
        insertion_sort(&stats, A, size);
        printf("Insertion Sort, %u elements, %lu moves, %lu compares", size, stats.moves,
            stats.compares);
        for (uint32_t index = 0; index < elements; index++) {
            if (index % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, A[index]);
        }
        printf("\n");
        reset(&stats);
        for (uint32_t array_index = 0; array_index < size; array_index++) {
            A[array_index] = B[array_index];
        }
    }
    if (member_set(QUICK, sorts)) {
        quick_sort(&stats, A, size);
        printf(
            "Quick Sort, %u elements, %lu moves, %lu compares", size, stats.moves, stats.compares);
        for (uint32_t index = 0; index < elements; index++) {
            if (index % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, A[index]);
        }
        printf("\n");
        reset(&stats);
        for (uint32_t array_index = 0; array_index < size; array_index++) {
            A[array_index] = B[array_index];
        }
    }
    free(A);
    free(B);
}
