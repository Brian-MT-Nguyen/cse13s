#include "shell.h"

#include "stats.h"

#include <math.h>
#include <stdio.h>

//Generates the gap size to be used for shell_sort (pseudo-generator)
//
//n: the size of the array to use for gap size calculations
static uint32_t value = 0;
uint32_t gaps(uint32_t n) {
    if (value <= 0) {
        value = log(2 * n + 3) / log(3);
    } else {
        value--;
    }
    return (pow(3, value) - 1) / 2;
}

//Iterates through the array given a gap size and moves elements around in ascending order given the input of a pointer to the array which directly manipulates and returns nothing
//
//stats: to use the stats.c functions to calculate the moves and comparisons
//A: the array that will be directly maniuplated and sorted
//n: the size or extent to which it will be sorted
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
    value = 0;
}
