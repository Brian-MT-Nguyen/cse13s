CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -O2
OBJECTS = speck.o ht.o bst.o node.o bf.o bv.o parser.o wbh.o

all: wbh

wbh: $(OBJECTS)
	$(CC) -o wbh $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f wbh $(OBJECTS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
