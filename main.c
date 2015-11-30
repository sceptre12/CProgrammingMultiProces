#include "header.h"

int main (int argc, char* argv [])
{
        char cmdline[BUFFERSIZE];

        for(;; ) {
                printf("COP4338$ ");
                if(fgets(cmdline, BUFFERSIZE, stdin) == NULL) {
                        perror("fgets failed");
                        exit(1);
                }
                execute(cmdline);
        }
        return 0;
}
