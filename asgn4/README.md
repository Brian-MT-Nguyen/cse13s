# Assignment 4 - The Perambulations of Denver Long

## Description
This program contains a collection of abstract data types(ADT) that work together to be used as a pathfinding algorithm. The main function calls on the other 3 ADTs to return the most optimal path (and all the paths given an input) using an adjancecy matrix graph, stack, and path of vertices.

## How to Build
1. Must have all 3 ADTs (graph.c, stack.c, path.c) [along with its header files], the vertices header file, and the main test function tsp.c in the same directory and folder as well as "Makefile" in order to construct and compile the program via clang with ease.
2. With these files in the same folder, one must construct and compile the library to run it with:
```
$ make
```

## How to Run
1. Once the command has been successfully executed, run the program with:
```
$ ./tsp <flag>

flag:
-h displays help msg prompt; -v enables verbose printing (prints out all paths); -u specifies the graph to be undirected; -i infile specifies the input file used to store cities and edges of the graph (default is stdin); -o outfilespecifies the output path file to print to (default is stdout)
```
2. If unsure of what command to input, one can either not include a flag and run the command, or include the -h flag to see the help documentation.


NOTE: scan-build has no errors so everything should work as intended.
