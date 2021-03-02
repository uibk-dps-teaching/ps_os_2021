CFLAGS = -std=gnu11 -Wall -Werror -Wextra

.PHONY: all clean

all: program1 program2

clean:
	$(RM) program1 program2 common.o

program1: program1.c common.h common.o

program2: program2.c common.h common.o
