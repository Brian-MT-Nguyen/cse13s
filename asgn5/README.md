# Assignment 5 - Huffman

## Description
This program contains an implementation of the Huffman encoder and decoder used to compress a data file. With this, the encoder has the job of compressing the file as it receives and reads an input then assigns the least amount of bits to the most common symbol and the greatest amount of bits to the least common symbol. The decoder does the opposite by reverting the compressed file back to its original raw size. 

## How to Build
1. Must have all .c and .h files asscociated with program and "Makefile".
2. With these files in the same folder, one must construct and compile the library to run it with:
```
$ make
or
$ make encode
or
$ make decode
```

## How to Run
1. Once the command has been successfully executed, run the encode program with:
```
$ ./encode <flag>

flag:
-h displays help msg prompt; -v prints out compression stats to stderr and shows file sizes; -i infile specifies the input file (default is stdin); -o outfile specifies the output path file to print to (default is stdout)
```
2. To run the decode program use:
```
$ ./decode <flag>

flag:
-h: displays help msg prompt; -v prints out decompression stats to stderr and shows file sizes; -i infile specifies the input file (default is stdin); -o outfile specifies the output path file to print to (default is stdout)
3. If unsure of what command to input, one can either not include a flag and run the command, or include the -h flag to see the help documentation.


NOTE: scan-build has no errors so everything should work as intended.
