#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

#define MAX_ARGS 8
void parse_args(char* str, char* (*result)[MAX_ARGS]) {
	memset(*result, 0, sizeof(char*) * MAX_ARGS);
	for(int i = 0; i < MAX_ARGS - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}

int main(int argc, char* argv[]) {

    pid_t pid;
    int file_descriptor[2];
    char keywords_first[MAX_ARGS][25];
    char keywords_sec[MAX_ARGS][25];

    if (argc < 2) {
        fprintf(stderr, "Incorrect call format. Please call program with at least one argument, i.e. keyword.");
        return EXIT_FAILURE;
    }

    parse_args(argv[1], &keywords_first);
    parse_args(argv[2], &keywords_sec);

    //create pipe
    if(pipe(file_descriptor)) {
        fprintf(stderr, "Pipe failed");
        return EXIT_FAILURE;
    }

    //fork a child process
    pid = fork();

    //catch error
    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return EXIT_FAILURE;
    } else if(pid == 0) 
    { //child process
        dup2(file_descriptor[WRITE_END], STDOUT_FILENO); //STDOUT_FILENO is the file descriptor for stdout stream
        close(file_descriptor[READ_END]);
        close(file_descriptor[WRITE_END]);
        execlp("/bin/ls", "ls", NULL);
        fprintf(stderr, "execution failed.");
        exit(EXIT_FAILURE);
    } else 
    { //parent process
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork failed");
            return EXIT_FAILURE;
        } else if(pid == 0){
            dup2(file_descriptor[READ_END], STDIN_FILENO);
            close(file_descriptor[WRITE_END]);
            close(file_descriptor[READ_END]);
            execlp("/bin/grep", "grep", keyword, NULL);
            fprintf(stderr, "grep execution failed.");
            exit(EXIT_FAILURE);
        } else {
            close(file_descriptor[READ_END]);
            close(file_descriptor[WRITE_END]);
            wait(NULL); //wait for children to complete
            printf("complete\n");
        }
    }
    return EXIT_SUCCESS;
}

