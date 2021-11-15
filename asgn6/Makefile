CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -O2 $(shell pkg-config --cflags gmp)
LDFLAGS = $(shell pkg-config --libs gmp)
E_OBJECTS = randstate.o numtheory.o rsa.o encrypt.o
D_OBJECTS = randstate.o numtheory.o rsa.o decrypt.o
K_OBJECTS = randstate.o numtheory.o rsa.o keygen.o

all: keygen encrypt decrypt

keygen: $(K_OBJECTS)
	$(CC) $(LDFLAGS) -o keygen $(K_OBJECTS)

encrypt: $(E_OBJECTS)
	$(CC) $(LDFLAGS) -o encrypt $(E_OBJECTS)

decrypt: $(D_OBJECTS)
	$(CC) $(LDFLAGS) -o decrypt $(D_OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt $(K_OBJECTS) $(E_OBJECTS) $(D_OBJECTS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
