#include "quick.h"

#include "stats.h"

#include <stdio.h>

//Divides the array into two divisions and sorts the array by swapping elements based on if it is less or greater than the pivot indicated by where the array is split
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: array that is being directly manipulated and sorted
//lo: provides the beginning index of where to sort the array
//hi: provides the end index of where to stop sorting
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

//Recursive function that uses partitions to divide the array then recursively sorts both sides
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: array that is being directly manipulated and sorted
//lo: provides the beginning index of where to sort the array
//hi: provides the end index of where to stop sorting
void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

//Calls on quick sorter to do the recursive sorting (the main function of the sort so to speak)
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: array that is being directly manipulated and sorted
//n: size of array to be sorted
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
