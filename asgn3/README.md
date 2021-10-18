# Assignment 3 - Sorting

## Description
This contains a collection of programs that all contain implemented sorting functions that sort a n-element array of randomly generated 30-bit unsigned integers. They print out the sorted array when prompted as well as its statistics on its moves and compares.

## How to Build
1. Must have all 4 math functions (insert.c, heap.c, quick.c, sort.c) [along with its header files], the stats function (stats.c) [along with its header file], and the main test function sorting.c in the same directory and folder as well as "Makefile" in order to construct and compile the program via clang with ease.
2. With these files in the same folder, one must construct and compile the library to run it with:
```
$ make
```

## How to Run
1. Once the command has been successfully executed, run the program with:
```
$ ./sorting <flag>

flag:
-a runs all sorts; -e runs heap sort; -i runs insertion sort; -s runs shell sort; -q runs quick sort; -r (seed) sets the random seed; -n (size) sets array size; -p (elements) prints out number of elements of the array; -h displays help msg prompt
```
2. If unsure of what command to input, one can either not include a flag and run the command, or include the -h flag to see the help documentation.


NOTE: scan-build has no errors so everything should work as intended.

