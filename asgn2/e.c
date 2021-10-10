#include "mathlib.h"
#include <stdio.h>

static int computed_terms = 1;
double total = 1;
double term = 1;

double e(void) {
	for (int k = 1; term > EPSILON; k++) {
		term *= (1.0/k);
		total += term;
		computed_terms++;
	}
	return total;
}

int e_terms(void) {
	return computed_terms;
}
