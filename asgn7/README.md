# Assignment 7 - The Great Firewall of Santa Cruz

## Description
This assignment contains a program that implements a message filtering system with the use of a bloom filter, hash table, node, bit vector, and binary search tree ADTS. The bloom filter gets the hash of a word using salts provided by the assignment and indexes them with the use of bit vectors. The hash table incorporates binary search tree (made of nodes) which contains all of the badspeak or newspeak words that will be filtered or punished in the message also using hashing similar to the bloom filter. This is all then used in the main banhammer program which either punishes or rewards the user for expressing a bad or good message respectively.

## How to Build
1. Must have all .c and .h files asscociated with said program and "Makefile".
2. With these files in the same folder, one can construct and compile all programs to build it with:
```
$ make
```
or
```
$ make all
```
3. To construct and compile just the banhammer program, build it with:
```
$ make banhammer
```

## How to Run
Once the program has been successfully compilied and built, one can run it with:
1. To run the banhammer program use:
```
$ ./banhammer <flag>

flag: -h Displays help message, -t sets the hash table size, -f sets the bloom filter size, -s Displays stats of the program
```
If unsure of what command to input, one can use the -h flag to see the help documentation of the program.


NOTE: scan-build has no errors so everything should work as intended.

