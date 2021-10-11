# Assignment 2 - A Little Slice of Pi

## Description
This contains a collection of programs that all contain math functions that try to approximate the math constants pi and e as well as the square root operation, essentially being a created math library. It also compares them to the built-in math.h library and outputs the differences and both the built-in library and this library side by side.

## How to Build
1. Must have all 6 math functions (bbp.c, e.c, euler.c, madhava.c, newton.c, and viete.c) and the main  test function mathlib-test.c in the same directory and folder as well as "Makefile" in order to construct and compile the program via clang with ease.
2. With 7 of the files in the same folder, one must construct and compile the library to run it with:
```
$ make
```

## How to Run
1. Once the command has been successfully executed, run the program with:
```
$ ./mathlib-test <flag>

flag:
-a runs all programs; -e runs e.c; -b runs bbp.c; -m runs madhava.c; -r runs euler.c; -v runs viete.c; -n runs newton.c; -s runs the terms of specificed program (Requires additional arg); -h displays help msg prompt
```
2. If unsure of what command to input, one can either not include a flag and run the command, or include the -h flag to see the help documentation.


NOTE: scan-build has no errors so everything should work as intended.

