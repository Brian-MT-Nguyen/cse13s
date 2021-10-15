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

typedef enum { INSERTION, HEAP, QUICK, SHELL, PRINT, HELP } Sorts;

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
        case 'p':
            sorts = insert_set(PRINT, sorts);
            elements = atoi(optarg);
            break;
        case 'h': sorts = insert_set(HELP, sorts); break;
        }
    }

    srand(seed);
    uint32_t bitmask = 0x3FFFFFFF;
    uint32_t *A = (uint32_t *) calloc(size, sizeof(uint32_t));

    if (elements > size) {
        elements = size;
    }
    for (uint32_t i = 0; i < size; i++) {
        A[i] = (random() & bitmask);
    }

    if (member_set(HELP, sorts) || sorts == empty_set()) {
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n\n");
        printf("USAGE\n");
        printf("   $ ./mathlib-test [-aebmrvnsh]\n\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all tests.\n");
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
        for (uint32_t i = 0; i < elements; i++) {
            printf("");
        }
    }
    if (member_set(SHELL, sorts)) {
        for (uint32_t i = 0; i < elements; i++) {
            printf("");
        }
    }
    if (member_set(INSERTION, sorts)) {
        for (uint32_t i = 0; i < elements; i++) {
            printf("");
        }
    }
    if (member_set(QUICK, sorts)) {
        for (uint32_t i = 0; i < elements; i++) {
            printf("");
        }
    }
    if (member_set(PRINT, sorts)) {
        for (uint32_t i = 0; i < elements; i++) {
            if (i % 5 == 0 && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, A[i]);
        }
        printf("\n");
    }
    free(A);
}
