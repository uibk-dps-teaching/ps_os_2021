CFLAGS = -std=c11 -Wall -Werror -Wextra -O2 -g

.PHONY: all
all: task1_tests task2_tests

.PHONY: clean
clean:
	$(RM) task1_tests task2_tests bstree.o

bstree.o: bstree.h bstree.c
	$(CC) $(CFLAGS) -c bstree.c -o $@

task1_tests: bstree.o task1_tests.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: check_task1
check_task1: task1_tests
	bash ./check_task.sh task1_tests

task2_tests: bstree.o task2_tests.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: check_task2
check_task2: task2_tests
	bash ./check_task.sh task2_tests
