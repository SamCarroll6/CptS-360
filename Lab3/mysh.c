#include "header.h"

int main(int argc, char* argv[], char* env[])
{
    int i = 0;
    char *input;
    char *Prog;
    char **args;
    // Find my process ID for parent.
    pid_t pid = getpid();
    printf("%d\n", pid);
    printf("************* Welcome to SamSH **************\n");
    // Print env directories (Home and Path)
    Showenv(env);
    printf("*********** SamSH processing loop **********\n");
    while(1)
    {
        input = readinput();
        args = Parse(input);
        RunProg(env, args, input);
        //free(input);
        //free(args);
        // while(args[i])
        // {
        //     args[i] = NULL;
        //     i++;
        // }
    }
}