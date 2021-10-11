#include "mathlib.h"

#include <stdio.h>

static int computed_terms = 1;

double pi_bbp(void) {
    double previousval = 0;
    double currentval = 47.0 / 15.0;
    double top = 1.0;

    for (int k = 1; absolute(currentval - previousval) > EPSILON; k++) {
        previousval = currentval;
        top = 1;
        for (int pow = 1; pow <= k; pow++) {
            top = top * 16;
        }
        currentval = currentval
                     + ((1 / top) * (k * (120 * k + 151) + 47)
                         / (k * (k * (k * (512 * k + 1024) + 712) + 194) + 15));
        computed_terms++;
    }
    return currentval;
}

int pi_bbp_terms(void) {
    return computed_terms;
}
