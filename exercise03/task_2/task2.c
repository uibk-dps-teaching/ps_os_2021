#define _POSIX_C_SOURCE 200809
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct sigaction newAct;

static void handler() {
	printf("Parent done.\n");
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Argument missing. This program needs to be called in the following format: ./task2 t (t = time in sec the child process sleeps) \nPlease try again.\n");
        return EXIT_FAILURE;
    }
    int t = atoi(argv[1]);
	pid_t pid = fork();
	if(pid < 0) {
		perror("Can't fork.");
		return EXIT_FAILURE;
	} else if(pid == 0) {
		// child process
		printf("Child %d sleeping for %d seconds...\n", getpid(), t);
		sleep(t);
		printf("Child done\n");
		exit(0);
	} else {
		// parent process
		newAct.sa_handler = handler;

		sigaction(SIGCHLD, // Signal sent when child process terminates
		          &newAct, // new action to be taken when the signal arrives
		          NULL);   // old action - not to be stored, therefore NULL

		// does not proceed until signal is handled
		pause();
	}

	return EXIT_SUCCESS;
}