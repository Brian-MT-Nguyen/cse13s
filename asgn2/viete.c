#include "mathlib.h"
#include <stdio.h>

static int counted_factors = 1;
double previousval = 0;
double currentval = sqrt_newton(2)/2;
double inside_sqrt = sqrt_netwon(2);

double pi_viete (void) {
	for (int k = 2; absolute(currentval - previousval) > EPSILON; k++) {
		previousval = currentval;
		inside_sqrt = sqrt_newton(2 + inside_sqrt);
		currentval *= (inside_sqrt/2);
		counted_factors++;
	}
	return 2.0/currentval;
}

int pi_viete_factors (void) {
	return counted_factors;
}
