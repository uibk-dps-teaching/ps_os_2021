#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        perror("This program expects one argument, which is the name of the client to be created.\n Example call: ./task1_client clientName");
        return EXIT_FAILURE;
    }

    int file_descriptor;
    FILE *file_pointer;
    char client_fifo[60] = "/tmp/client_";
    char buffer_str[PIPE_BUF];

    strcat(client_fifo, argv[1]);

    printf("Name: %s\n", client_fifo);

    if ((file_pointer = fopen(client_fifo, "w")) == NULL) {
        perror("Write:");
        exit(EXIT_FAILURE);
    } else {
        file_descriptor = fileno(file_pointer);
    }

    write(file_descriptor, argv[1], sizeof(argv[1]));

    int run = 1;
    while(run){
        printf("Message:\n>");
        fgets(buffer_str, PIPE_BUF, stdin);
        if(buffer_str[0] == 10) { //10 = '\n'
            run = 0; //break loop
        }

        /* write buffer_str to the FIFO */
        write(file_descriptor, buffer_str, sizeof(buffer_str));

        // clean buffer from any data
        memset(buffer_str, 0, sizeof(buffer_str));
    }

    /* remove the FIFO */
    fclose(file_pointer);

    return EXIT_SUCCESS;
}