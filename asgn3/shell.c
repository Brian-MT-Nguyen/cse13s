#include "shell.h"

#include "stats.h"

#include <math.h>
#include <stdio.h>

uint32_t gaps(uint32_t n) {
    static uint32_t value = 0;
    if (value <= 0) {
        value = log(2 * n + 3) / log(3);
    } else {
        value--;
    }
    return (uint32_t)((pow(3, value) - 1) / 2);
}

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t max_gap = log(2 * n + 3) / log(3);
    for (uint32_t gap_changer = 0; gap_changer < max_gap; gap_changer++) {
        uint32_t gap = gaps(n);
        for (uint32_t index = gap; index < n; index++) {
            uint32_t compare_index = index;
            uint32_t temp = move(stats, A[index]);
            while (compare_index >= gap && (cmp(stats, temp, A[compare_index - gap]) == -1)) {
                A[compare_index] = move(stats, A[compare_index - gap]);
                compare_index -= gap;
            }
            A[compare_index] = move(stats, temp);
        }
    }
}
