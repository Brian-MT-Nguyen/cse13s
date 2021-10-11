#include "mathlib.h"

#include <stdio.h>

static int computed_terms = 1;

double pi_euler(void) {
    double previousval = -1;
    double currentval = 1;
    double term = 1.0;

    for (long int k = 2; absolute(currentval - previousval) > EPSILON; k++) {
        previousval = currentval;
        term = 1.0 / (k * k);
        currentval += term;
        computed_terms++;
    }

    return sqrt_newton(6 * currentval);
}

int pi_euler_terms(void) {
    return computed_terms;
}
