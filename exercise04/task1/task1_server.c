#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    // exit program if called without a client
    if (argc == 1) {
        perror(
                "Please call the programm with the names of the client servers you would like to "
                "connect to. \ne.g. ./task1_server client1 client2  - with a minimum of one client\n ");
        return EXIT_FAILURE;
    }

    char fifo_names[argc - 1][60]; //a char array that contains the names of the fifos

    int max_fd;

    int nr_ready_files; 
    int file_descriptor[argc - 1]; //array containing all the filedescriptors
    fd_set read_fds; // file descriptors for the fifos that are ready to be read
    fd_set connected_fds; // file descriptors of all connected client fifos

    char buf[PIPE_BUF];
    memset(buf, 0, sizeof(buf));

    FD_ZERO(&read_fds); // clear file descriptor set for select

    // creating the paths for the clients FIFO's
    char fifo_path[60] = "/tmp/client_";

    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) >= 48) {
            printf("The %d. server name \"%s\" is too long. Please choose a name of 48 characters "
                   "or less.",
                   i, argv[i]);
            exit(EXIT_FAILURE);
        }
        strcpy(fifo_names[i - 1], fifo_path);
        strcat(fifo_names[i - 1], argv[i]);
        printf("%s\n", fifo_names[i - 1]);
    }

    //-----------------------------------------------------------------------------
    for (int i = 0; i < argc - 1; i++) {
        // create the fifos
        if ((mkfifo(fifo_names[i], 0666)) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        // open the fifos and store FILE* in array
        if ((file_descriptor[i] = open(fifo_names[i], O_RDONLY)) == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        /*
        error handling..
        */
        max_fd = max_fd > file_descriptor[i] ? max_fd : file_descriptor[i];
        //printf("%s connected\n\n", argv[i + 1]);
        printf("made fifos for %s\n", argv[i + 1]);
        FD_SET(file_descriptor[i], &connected_fds);
    }

    //-----------------------------------------------------------------------------
    int run = 1;
    while (run) {
        int response;
        read_fds = connected_fds;
        nr_ready_files = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (nr_ready_files == 0) {
            printf("Clean and exit.");
            run = 0;
            break;
        } else {
            for (int i = 0; i < argc - 1; i++) {
                if (FD_ISSET(file_descriptor[i], &read_fds)) {
                    response = read(file_descriptor[i], buf, sizeof(buf));
                    if (strcmp(buf, argv[i + 1]) == 0) {
                        printf("%s connected\n", buf);
                    } else if (response > 0) {
                        printf("%s: %s\n\n", argv[i + 1], buf);
                    } else {
                        if (file_descriptor[i] == -1) continue;
                        printf("%s disconnected\n\n", argv[i + 1]);
                        //remove fifo from connected files
                        FD_CLR(file_descriptor[i], &connected_fds);
                        //close fifo
                        if (close(file_descriptor[i] == EOF)) {
                            perror("close");
                            exit(EXIT_FAILURE);
                        } else {
                            file_descriptor[i] = -1;
                        }
                    }
                }
                // clean buf from any data
                memset(buf, 0, sizeof(buf));
            }
        }
    }

    for(int i = 0; i < argc - 1; i++){
        if (unlink(fifo_names[i]) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}