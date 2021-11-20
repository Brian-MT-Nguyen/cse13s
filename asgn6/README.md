# Assignment 5 - Huffman

## Description
This program contains an implementation of the Huffman encoder and decoder used to compress a data file. With this, the encoder has the job of compressing the file as it receives and reads an input then assigns the least amount of bits to the most common symbol and the greatest amount of bits to the least common symbol. The decoder does the opposite by reverting the compressed file back to its original raw size. 

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
3. To construct and compile just the key generator program, build it with:
```
$ make keygen
```
4. To construct and compile just the encrypt program, build it with:
```
$ make encrypt
```
5. To construct and compile just the decrypt program, build it with:
```
$ make decrypt
```

## How to Run
Once the program(s) have been successfully compilied and built, one can run up to 3 programs:
1. To run the keygen program use:
```
$ ./keygen <flag>

flag:
-b sets bits needed for public modulus n (default: 256), -i sets amount of Miller-Rabin iterations used to test primes (default: 50), -n specifies file to write public key to (default: rsa.pub), -d specifies file to write private key to (default: rsa.priv), -s sets seed used for random number generation (default: time(NULL)), -v prints verbose output, -h displays help synopsis and usage.
```
2. To run the encrypt program use:
```
$ ./encrypt <flag>

flag:
-i specifies input file to encrypt from (default: stdin), -o specifies output file to encrypt to (default: stdout), -n specifies file containing public key (default: rsa.pub), -v prints verbose output, -h displays help synopsis and usage.
```
3. To run the decrypt program use:
```
$ ./decrypt <flag>

flag:
-i specifies input file to decrypt from (default: stdin), -o specifies output file to decrypt to (default: stdout), -n specifies file containing private key (default: rsa.priv), -v prints verbose output, -h displays help synopsis and usage.
```
If unsure of what command to input, one can use the -h flag on any of the programs to see the help documentation of the respective programs.


NOTE: scan-build has no errors so everything should work as intended.
