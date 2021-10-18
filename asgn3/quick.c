#include "quick.h"

#include "stats.h"

#include <stdio.h>

uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t lo_pointer = lo - 1;
    for (uint32_t hi_pointer = lo; hi_pointer < hi; hi_pointer++) {
        if (cmp(stats, A[hi_pointer - 1], A[hi - 1]) == -1) {
            lo_pointer += 1;
            swap(stats, &A[lo_pointer - 1], &A[hi_pointer - 1]);
        }
    }
    swap(stats, &A[lo_pointer], &A[hi - 1]);
    return lo_pointer + 1;
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
