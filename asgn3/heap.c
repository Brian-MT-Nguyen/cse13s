#include "heap.h"

#include "stats.h"

#include <stdbool.h>
#include <stdio.h>

//Returns the value of the greatest child node (left or right)
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: the array that is directly manipulated and sorted
//first: specifies the first index of the array
//last: specifies the last index of the array
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && (cmp(stats, A[right - 1], A[left - 1]) == 1)) {
        return right;
    }
    return left;
}
//If the child node is greater than its parent node it swaps positions
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: the array that is directly manipulated and sorted
//first: specifies the first index of the array
//last: specifies the last index of the array
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while (mother <= ((uint32_t) last / 2) && !found) {
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
}

//Calls on fix heap to re-sort or re-build the array after removing elements or swapping it out
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: the array that is directly manipulated and sorted
//first: specifies the first index of the array
//last: specifies the last index of the array
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = last / 2; father > first - 1; father--) {
        fix_heap(stats, A, father, last);
    }
}

//Calls on build heap to build the array, then swaps values based on if its greater than others, then fixes the heap to repeat the process
//
//stats: to use the stats.c functions to calculate moves and comparisons
//A: the array that is directly manipulated and sorted
//first: specifies the first index of the array
//last: specifies the last index of the array
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf--) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
}
