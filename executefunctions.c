#include "header.h"

void standardSettings(int rc, char *command, char *newArgs[]){
    if(rc == -1) {
            perror("pipe");
            exit(1);
    }

    int nargs = get_args(command,newArgs);

    if(SHOWLOGS) {
            printf("Arg0: '%s'\n",newArgs[0]);
            int s = 0;
            for(s = 0; s < nargs; s++) {
                    printf("Arguments: '%s'\n",newArgs[s]);
            }
    }
}
void firstPipe(char *args[],int data_pipe[]){

    if(SHOWLOGS)
        printf("arg0: '%s'\n", args[0]);

    // Closes the read file descriptor
    close(data_pipe[0]);

    // listens on the output from stdout
    dup2(data_pipe[1], STDOUT_FILENO);

    // closes the old write file descriptor
    close(data_pipe[1]);

    if(execvp(args[0], args) < 0){
        printf("*** ERROR: exec failed\n");
               exit(1);
    }
    close(data_pipe[1]);
}
void otherPipe(char *args[],int data_pipe[]){
    if(SHOWLOGS)
        printf("arg0: '%s'\n", args[0]);
    // Closes the write file descriptor
    close(data_pipe[1]);

    // listens on the input from stdin
    dup2(data_pipe[0], STDIN_FILENO);

    close(data_pipe[0]);
    // closes the old write file descriptor

    if (execvp(args[0], args) < 0){ // execute the program
        printf("*** ERROR: exec failed\n");
               exit(1);
    }
    close(data_pipe[0]);
}
