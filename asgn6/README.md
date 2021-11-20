# Assignment 6 - Public Key Cryptography

## Description
This collection of programs implement the RSA public-key cryptosystem with the structure of a key generator, encryptor, and decryptor. The key generator produces an RSA private file and public key file. The encryptor uses the public key to encrypt files, and the decryptor uses the private key to decrypt the encrypted file. These programs together are used to securely encrypt and decrypt messages or files sent between clients.

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
