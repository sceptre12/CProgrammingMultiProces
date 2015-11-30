#include "header.h"

void execute(char* cmdline)
{
        int pid, async;
        int commandListLength =0; // length of the commands that parsed
        char *args[MAX_ARGS];
        char *listOfCommands[MAX_ARGS]; // List of the full commands
        char *randp[MAX_ARGS]; // list of the redirection commands
        // char *separateCommand[2]; // first index is the command to run the second is its args

        //Gets the number of arguments
        int nargs = get_args(cmdline, args);
        if(nargs <= 0) return;

        if(SHOWLOGS)
                printf("nargs: '%d'\n",nargs);

        listOfArgs(args,nargs,listOfCommands, &commandListLength, randp);

        if(SHOWLOGS)
                printf("COmmand Length: '%d'\n", commandListLength);

        // Stops the program
        if(!strcmp(args[0], "quit") || !strcmp(args[0], "exit")) {
                exit(0);
        }

        /* check if async call */
        if(!strcmp(args[nargs-1], "&")) { async = 1; args[--nargs] = 0; }
        else async = 0;

        if(commandListLength == 0) {
                pid = fork();
                if(pid == 0) { /* child process */
                        execvp(args[0], args);
                        /* return only when exec fails */
                        perror("exec failed");
                        exit(-1);
                } else if(pid > 0) { /* parent process */
                        if(!async) waitpid(pid, NULL, 0);
                        else printf("this is an async call\n");
                } else { /* error occurred */
                        perror("fork failed");
                        exit(1);
                }
        }else{
                if(SHOWLOGS)
                        printf("Multiple commands have been passed\n");

                int data_pipe[2]; // array storing the file descriptors
                int childProcessArray[commandListLength]; // Array Storing the child process
                int childStatusArray[commandListLength]; // status of the child processes
                int rc = 0; // return value of the pipe
                int position = 0; // current index in the while loop
                int randIndex  = 0; // current index for the redirection and pipe array


                /*
                    The first pipe[0] will contain the number its read file descriptor
                    the second pipe[1] will contain the number of its write file descripter
                 */
                rc = pipe(data_pipe);

                while(position < commandListLength) {
                        if(SHOWLOGS) {
                                printf("Inside of While loop\n");
                                printf("index: '%d'\n", position);
                        }

                        char *newArgs[MAX_ARGS]; // New list of args
                        static int iteration = 0;
                        // the index of randp needs to be seperate
                        char *compare = randp[randIndex];
                        if(SHOWLOGS)
                                printf("compare: '%s'\n", compare);
                        if(strcmp(compare, "|") == 0) {
                                standardSettings(rc,listOfCommands[position],newArgs);

                                if(iteration == 0) {
                                        if(SHOWLOGS)
                                                printf("Inside of iteration 0\n");

                                        childProcessArray[position] = fork();

                                        if(childProcessArray[position] == 0) {
                                                if(SHOWLOGS)
                                                        printf("Inside of Child process\n");

                                                        firstPipe(newArgs,data_pipe);

                                                _exit(EXIT_SUCCESS);
                                        }else if(childProcessArray[position] == -1) {
                                                perror("fork child Error");
                                                exit(1);
                                        }else{
                                            wait(&childStatusArray[position]);
                                        }

                                        iteration = 1;
                                }else{
                                        if(SHOWLOGS)
                                                printf("Inside of iteration: '%d'\n",iteration);

                                        childProcessArray[position] = fork();

                                        printf("Command string: '%s'\n", listOfCommands[position]);

                                        if(childProcessArray[position] == 0) { /* child process */
                                                if(SHOWLOGS)
                                                        printf("Inside of %d child process\n", position);
                                                        otherPipe(newArgs, data_pipe);
                                                _exit(EXIT_SUCCESS);
                                        }else if (childProcessArray[position] == -1) {
                                                perror("fork child Error");
                                                exit(1);
                                        }else{
                                                wait(&childStatusArray[position]);
                                        }

                                        randIndex++;
                                        iteration = 0;
                                }

                        }else if (strcmp(compare, ">") == 0) {

                                // int a = 0;
                                //
                                // if(SHOWLOGS)
                                //         printf("Setting all args to be empty\n");
                                //
                                // for(a =0; a < MAX_ARGS; a++) {
                                //         args[a] = "";
                                // }
                                //
                                // if(SHOWLOGS) {
                                //         printf("Getting a new list of args\n");
                                //         printf("Coming from: '%s'\n", listOfCommands[position]);
                                // }
                                //
                                //
                                // nargs = get_args(listOfCommands[position], args);
                                //
                                // if(SHOWLOGS) {
                                //         printf("Arg0: '%s'\n",args[0]);
                                //         int s = 0;
                                //         for(s = 0; s < nargs; s++) {
                                //                 printf("Arguments: '%s'\n",args[s]);
                                //         }
                                // }
                                //
                                // if(iteration == 0){
                                //     if(SHOWLOGS)
                                //             printf("Inside of iteration 0\n");
                                //     childProcessArray[position] = fork();
                                //     switch(childProcessArray[position]) {
                                //     case 0:
                                //             if(SHOWLOGS)
                                //                     printf("Inside of Child process\n");
                                //
                                //             // Closes the read file descriptor
                                //             close(data_pipe[0]);
                                //
                                //             // listens on the output from stdout
                                //             dup2(data_pipe[1], STDOUT_FILENO);
                                //
                                //             // closes the old write file descriptor
                                //             close(data_pipe[1]);
                                //
                                //             execvp(args[0], args);
                                //             exit(0);
                                //     case -1:
                                //             perror("fork child Error");
                                //             exit(1);
                                //     default:
                                //             break;
                                //     }
                                //     iteration = 1;
                                // }else{
                                //     if(SHOWLOGS)
                                //         printf("Inside of iteration: '%d'\n", iteration);
                                //         childProcessArray[position] = fork();
                                //         switch(childProcessArray[position]) {
                                //         case 0:
                                //                 if(SHOWLOGS)
                                //                         printf("Inside of Child process\n");
                                //
                                //                 // Closes the write file descriptor
                                //                 close(data_pipe[1]);
                                //
                                //                 // listens on the input from STDIN_FILENO
                                //                 dup2(data_pipe[0], STDIN_FILENO);
                                //
                                //                 // closes the old write file descriptor
                                //                 close(data_pipe[0]);
                                //
                                //                 int f = open(listOfCommands[position], O_CREAT | O_RDWR);
                                //                 dup2(f,STDIN_FILENO);
                                //
                                //                 close(f);
                                //                 close(STDIN_FILENO);
                                //
                                //
                                //                 exit(0);
                                //         case -1:
                                //                 perror("fork child Error");
                                //                 exit(1);
                                //         default:
                                //                 break;
                                //         }
                                //         iteration = 0;
                                // }
                        }
                        position++;
                }
                close(data_pipe[0]);
                close(data_pipe[1]);
        }
}
