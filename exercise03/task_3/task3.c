#define _POSIX_C_SOURCE 200809
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct sigaction done, recieved;
volatile int breakflag = 1;

static void parent_handler() {
	printf("Parent done.\n");
	breakflag--;
	return;
}

static void usr2_handler() {
	printf("Child done.\n");
	exit(0);
	return;
}

static void usr1_handler() {
	printf("Recieved SIGUSR1.\n");
	return;
}

static void alarm_handler() {}

int main() {
	// create sigmask
	sigset_t usr2_mask;
	sigemptyset(&usr2_mask);
	sigaddset(&usr2_mask, SIGUSR2);

	sigset_t usr1_mask;
	sigemptyset(&usr1_mask);
	sigaddset(&usr1_mask, SIGUSR1);

	pid_t child_pid = fork();

	if(child_pid < 0) {
		printf("Can't fork.");
		return EXIT_FAILURE;
	} else if(child_pid == 0) {
		// child process
		sigprocmask(SIG_BLOCK, &usr2_mask, NULL);
		sleep(13);
		sigprocmask(SIG_UNBLOCK, &usr2_mask, NULL);
		recieved.sa_handler = usr1_handler;
		// recieved.sa_flags = SA_RESTART; // restart function if interrupted by handler
		done.sa_handler = usr2_handler;
		pause();
	} else {
		// parent process
		// set up alarm handler
		signal(SIGALRM, alarm_handler);

		done.sa_handler = parent_handler;

		alarm(3);
		pause();
		kill(child_pid, SIGUSR1);

		sigaction(SIGCHLD, &done, NULL);
		while(breakflag) {
			alarm(3);
			pause();
			kill(child_pid, SIGUSR2);
		};
	}
	return EXIT_SUCCESS;
}