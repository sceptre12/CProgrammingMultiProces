#include "header.h"

// gets the number of arguments
int get_args(char* cmdline, char* args[])
{
        int i = 0;
        /* if no args */

        if((args[0] = strtok(cmdline, "\n\t ")) == NULL)
                return 0;

        /*
            The ++i adds 1 to i and then adds i to itself
            the i++ adds i to itself so

            The reason strtok is NULL the second time is because
            you want to tell strtok to resume from the last spot in the string
         */
        while((args[++i] = strtok(NULL, "\n\t ")) != NULL) {
                if(i >= MAX_ARGS) {
                        printf("Too many arguments!\n");
                        exit(1);
                }
        }

        /* the last one is always NULL */
        return i;
}

int listOfArgs(char *args[],int nargs, char *listOfCommands[],int *commandListLength, char *randp[]){
        // if(SHOWLOGS) {
        //         printf("Inside of listArgs function\n");
        // }
        // only 1 args
        if(nargs == 1) return 1;
        int i = 1;
        int a = 0;
        int b = 0;
        int c = 0;


        // Index that determine " | < > "
        int shouldSplit[MAX_ARGS];

        for(i = 0; i < nargs; i++) {
                char *ret;
                ret = strpbrk(args[i], "|<>");
                if(ret) {
                        // printf("First matching character: %c\n", *ret);
                        shouldSplit[a] = i;
                        randp[a] = (char *)malloc(20 * sizeof(char));
                        randp[a] = args[i];
                        a++;
                }
        }
        // none of the args contain delims
        if(a == 0) return 1;

        for(i = 0; i < a + 1; i++) {
                listOfCommands[i] = (char *)malloc(20 * sizeof(char));
        }

        for(i = 0; i < nargs; i++) {
                for(b = 0; b < a; b++) {
                        // printf("shouldsplit: '%d'\n", shouldSplit[b]);
                        if(i == shouldSplit[b]) {
                                i++;
                                c++;
                                b = a;
                                // printf("arg: '%d', newArgs: '%d', b: '%d'\n",i,c,b);
                                // printf("C: '%d'\n", c);
                        }
                }
                if(i < nargs) {
                        strcat(listOfCommands[c], args[i]);
                        strcat(listOfCommands[c], " ");
                        // printf("String Test: '%s'\n",listOfCommands[c]);
                }
        }
        *commandListLength = c + 1;



        return 0;
}
void catchChild(int sig_num){
        int child_status;

        wait(&child_status);
        printf("child has exited: '%d'\n",child_status);
}

// no longer needed keeping just in case
int breakApartCommand(char command[],char *separateCommand[]){
        if(SHOWLOGS) {
                printf("Inside of breakApartCommand\n");
                printf("Command: '%s'\n", command);
        }


        int pos;
        char space[] = " ";
        pos = strcspn(command,space);

        if(SHOWLOGS)
                printf("First Space: '%d'\n", pos);


        separateCommand[0] = (char *)malloc(pos * sizeof(char));
        if(separateCommand[0] == NULL){
            exit(1);
        }

        memcpy(separateCommand[0],command,pos);

        char *start = &command[pos+1];
        char *end = &command[strlen(command)];

        // Allocates enough memory for the length of the remaining string and the null terminator
        separateCommand[1]= (char *)calloc(1,end - start +1);
        if(separateCommand[1] == NULL){
            exit(1);
        }
        memcpy(separateCommand[1],start, end - start);



        if(SHOWLOGS){
            printf("Index 0: '%s'\n",separateCommand[0]);
            printf("Index 1: '%s'\n",separateCommand[1]);
        }


        return 0;
}
