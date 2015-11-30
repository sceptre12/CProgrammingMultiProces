#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

//Define Declarations
#define MAX_ARGS 20
#define BUFFERSIZE 1024
#define SHOWLOGS 1 // Turns on showing the logs throughout the programing

//Function Declarations
int get_args(char* cmdline, char* args[]);
void execute(char* cmdline);

/*
    Retreives the command list, The length of that list as well as the array of redirection commands
*/
int listOfArgs(char *args[],int nargs,char *listOfCommands[],int *commandListLength, char *randp[]);

/*
    Breaks apart the command from the argument list
*/
int breakApartCommand(char command[],char *separateCommand[]);
void standardSettings(int rc, char *command,  char *newArgs[]);
void firstPipe(char *args[],int data_pipe[]);
void otherPipe(char *args[],int data_pipe[]);
// catches child death signal
void catchChild(int sig_num);
