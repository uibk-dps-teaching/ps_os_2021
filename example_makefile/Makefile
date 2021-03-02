CFLAGS = -std=gnu11 -Wall -Werror -Wextra

.PHONY: all clean

all: program1 program2

clean:
	$(RM) common.o program1 program2

program1: common.o program1.c

program2: common.o program2.c
