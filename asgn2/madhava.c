#include "mathlib.h"

#include <stdio.h>

static int computed_terms = 1;

double pi_madhava(void) {
    double previousval = 0;
    double currentval = 1;
    double top = 1;

    for (int k = 1; absolute(currentval - previousval) > EPSILON; k++) {
        previousval = currentval;
        top = 1;
        for (int pow = 1; pow <= k; pow++) {
            top = top * -3;
        }
        currentval = currentval + ((1 / top) / (2 * k + 1));
        computed_terms++;
    }
    return sqrt_newton(12) * currentval;
}

int pi_madhava_terms(void) {
    return computed_terms;
}
