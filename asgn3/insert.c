#include "insert.h"

#include "stats.h"

#include <stdio.h>
//Iterates through the array one by one and moves elements around in ascending order given the input of a pointer to the array which directly manipulates and returns nothing
//
//stats: to use the stats.c functions to calculate the moves and comparisons
//A: the array that will be directly maniuplated and sorted
//n: the size or extent to which it will be sorted
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    for (uint32_t index = 1; index < n; index++) {
        uint32_t compare_index = index;
        uint32_t temp = move(stats, A[index]);
        while (compare_index > 0 && (cmp(stats, temp, A[compare_index - 1]) == -1)) {
            A[compare_index] = move(stats, A[compare_index - 1]);
            compare_index -= 1;
        }
        A[compare_index] = move(stats, temp);
    }
}
