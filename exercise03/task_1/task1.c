//
// Created by Salma on 20.03.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

double mc_pi(int64_t S) {
    int64_t in_count = 0;
    for(int64_t i = 0; i < S; ++i) {
        const double x = rand() / (double)RAND_MAX;
        const double y = rand() / (double)RAND_MAX;
        if(x*x + y*y <= 1.f) {
            in_count++;
        }
    }
    return 4 * in_count / (double)S;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Argument missing. This program needs to be called in the following format: ./task1 N S (N = number of child processes, S = number of random samples to estimate PI) Please try again.\n");
        return EXIT_FAILURE;
    }
    int64_t n = atoi(argv[1]);
    int64_t s = atoi(argv[2]);
    for(int i = 0; i < n; i++) {
        if(fork() == 0) {
            printf("Child %d PID = %d. mc_pi(%ld) = %f\n", i, getpid(), s, mc_pi(s));
            exit(0);
        } else {
            wait(NULL);
        }
    }
    printf("Done\n");
    return EXIT_SUCCESS;
}

/*
The order of the output is consistent.
The printed children and PID's are in order, since they are called successively.
If multiple child processes are created simultaneously the order of the messages would be
inconsistent.
*/