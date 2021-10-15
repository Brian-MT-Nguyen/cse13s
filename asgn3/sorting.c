#include "insert.h"
#include "heap.h"
#include "quick.h"
#include "shell.h"
#include "stats.h"
#include "sets.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aeisqr:n:p:h"
#define SEED 13371453
#define SIZE 100

int main(int argc, char **argv) {
    int opt = 0

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        none = false;
        switch (opt) {
        case 'a': ; break;
        case 'e': ; break;
        case 'i': ; break;
        case 's': ; break;
        case 'q': ; break;
        case 'r': ; break;
        case 'n': ; break;
        case 'p': ; break;
        case 'h': ; break;
        }
    }
}
