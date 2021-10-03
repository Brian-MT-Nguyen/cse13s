CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: pig

pig: pig.o

pig.o: pig.c names.h

clean:
	rm -f pig pig.o

format:
	clang-format -i -style=file *.[ch]
