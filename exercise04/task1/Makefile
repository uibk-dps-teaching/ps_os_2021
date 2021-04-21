CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS = -lrt

.PHONY: all clean
all: task1_server task1_client task2 task3
clean: $(RM) task1_server task1_client task2 task3
task1_server: task1_server.c
task1_client: task1_client.c
task2: task2.c
task3: task3_server.c