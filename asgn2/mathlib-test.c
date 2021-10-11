#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
    int opt = 0;
    bool run_e = false;
    bool run_euler = false;
    bool run_bbp = false;
    bool run_madhava = false;
    bool run_newton = false;
    bool run_viete = false;
    bool run_stats = false;
    bool help = false;
    bool none = true;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        none = false;
        switch (opt) {
        case 'a': run_e = run_euler = run_bbp = run_madhava = run_newton = run_viete = true; break;
        case 'e': run_e = true; break;
        case 'b': run_bbp = true; break;
        case 'm': run_madhava = true; break;
        case 'r': run_euler = true; break;
        case 'v': run_viete = true; break;
        case 'n': run_newton = true; break;
        case 's': run_stats = true; break;
        case 'h': help = true; break;
        }
    }
    double pi = M_PI;
    double eu = M_E;
    if (help || none) {
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n\n");
        printf("USAGE\n");
        printf("   $ ./mathlib-test [-aebmrvnsh]\n\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all tests.\n");
        printf("  -e   Runs e pi test.\n");
        printf("  -b   Runs BBP pi test.\n");
        printf("  -m   Runs Madhava pi test.\n");
        printf("  -r   Runs Euler pi test.\n");
        printf("  -v   Runs Viete pi test.\n");
        printf("  -n   Runs Newton square root tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program synopsis and usage.\n");
    }
    if (run_e == true) {
        double my_e = e();
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", my_e, eu, absolute(my_e - eu));
    }

    if (run_euler == true) {
        double my_euler = pi_euler();
        printf("pi_euler() = %16.15lf, M_PI= %16.15lf, diff = %16.15lf\n", my_euler, pi,
            absolute(my_euler - pi));
    }
    if (run_bbp == true) {
        double my_bbp = pi_bbp();
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", my_bbp, pi,
            absolute(my_bbp - pi));
    }
    if (run_madhava == true) {
        double my_madhava = pi_madhava();
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", my_madhava, pi,
            absolute(my_madhava - pi));
    }
    if (run_viete == true) {
        double my_viete = pi_viete();
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", my_viete, pi,
            absolute(my_viete - pi));
    }

    if (run_newton == true) {
        for (double i = 0.0; i < 10; i += 0.1) {
            double my_newton = sqrt_newton(i);
            printf("sqrt_newton(%f) = %16.15lf, sqrt(%f) = %16.15lf, diff = %16.15lf\n", i,
                my_newton, i, sqrt(i), absolute(my_newton - sqrt(i)));
            if (run_stats == true) {
                int newton_terms = sqrt_newton_iters();
                printf("sqrt_newton() terms = %d\n", newton_terms);
            }
        }
    }
    if (run_stats == true) {
        if (run_e == true) {
            int terms_e = e_terms();
            printf("e() terms = %d\n", terms_e);
        }
        if (run_euler == true) {
            int euler_terms = pi_euler_terms();
            printf("pi_euler() terms = %d\n", euler_terms);
        }
        if (run_bbp == true) {
            int bbp_terms = pi_bbp_terms();
            printf("pi_bbp() terms = %d\n", bbp_terms);
        }
        if (run_madhava == true) {
            int madhava_terms = pi_madhava_terms();
            printf("pi_madhava() terms = %d\n", madhava_terms);
        }
        if (run_viete == true) {
            int viete_factors = pi_viete_factors();
            printf("pi_viete() terms = %d\n", viete_factors);
        }
    }
}
