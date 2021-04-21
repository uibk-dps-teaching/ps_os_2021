//
// Created by s on 21.04.20.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_SIZE 8192
#define QUEUE_NAME "/remoteCommandExecution"
#define READ_END 0
#define WRITE_END 1

int main() {

    mqd_t mq;
    pid_t pid;
    char command[MAX_SIZE];
    char output[BUFSIZ];
    int file_descriptor[2];
    ssize_t read_bytes;

    //create & open message queue
    if ((mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, NULL)) == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    struct mq_attr mq_attr;
    mq_getattr(mq, &mq_attr);

    //create pipe
    if (pipe(file_descriptor)) {
        fprintf(stderr, "Pipe failed");
        return EXIT_FAILURE;
    }
    memset(output, '\0', sizeof(output));
    memset(command, '\0', sizeof(command));

    //read and execute command from mq
    do {
        read_bytes = mq_receive(mq, command, MAX_SIZE, NULL);
        if (read_bytes == -1) {
            perror("mq_recieve");
            continue;
        } else if (read_bytes == 0) {
            printf("loop skipped... waiting for msg\n");
            continue;
        }
        if (!strcmp(command, "exit")) break; //exit loop

        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork failed");
            return EXIT_FAILURE;
        } else if (pid == 0) { //child
            //execute command and send stdout through pipe to parent
            dup2(file_descriptor[READ_END], STDIN_FILENO);
            close(file_descriptor[WRITE_END]);
            close(file_descriptor[READ_END]);
            execlp(command, command, NULL);
            fprintf(stderr, "%s execution failed.", command);
            exit(EXIT_FAILURE);
        } else { //parent
            //reads information from pipe and writes into mq
            close(file_descriptor[READ_END]);
            close(file_descriptor[WRITE_END]);
            wait(NULL); //wait for children to complete
            read(file_descriptor[READ_END], output, BUFSIZ);
            mq_send(mq, output, strlen(output), 0);
            printf("complete\n");
        }
    } while (1);

    mq_unlink(QUEUE_NAME);
    return EXIT_SUCCESS;
}
