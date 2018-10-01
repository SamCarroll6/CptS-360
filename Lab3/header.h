#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

char HOME[128];
char PATH[256];
char *pathdirs[64];

/*
 * Showenv, function takes the env variable from main parameters.
 * Prints the Home directory as well as Path as a whole and split
 * into individual directories. 
 */
int Showenv(char* env[])
{
    char* token;
    int i = 0, reghome = 0, regpath = 0;
    // Initialize regex for Home and Path variables.
    regex_t regHOME, regPATH;
    reghome = regcomp(&regHOME, "^HOME", REG_EXTENDED);
    regpath = regcomp(&regPATH, "^PATH=", REG_EXTENDED);
    // If regex worked
    if(!reghome && !regpath)
    {
        while(env[i] != NULL)
        {
            // Check that regex matches env variable
            reghome = regexec(&regHOME, env[i], 0, NULL, 0);
            regpath = regexec(&regPATH, env[i], 0, NULL, 0);
            if(!reghome)
            {
                strcpy(HOME, env[i]);
            }
            if(!regpath)
            {
                strcpy(PATH, env[i]);
            }
            i++;
        }
        regfree(&regHOME);
        regfree(&regPATH);
    }
    i = 0;
    // Print HOME and PATH as they are, as well as 
    // PATH broken into directory names.
    printf("3: Show Home Directory: %s\n", HOME);
    strcpy(HOME, strtok(HOME, "HOME="));
    token = strtok(PATH, "PATH=");
    printf("1. Show Path: \n%s\n", PATH);
    token = strtok(token, ":");
    pathdirs[i] = token;
    printf("2. Decompose Path into dir strings:\n%s ", token);
    while(token)
    {
        token = strtok(NULL, ":");
        i++;
        if(token)
        {
            printf("%s ", token);
        }
        pathdirs[i] = token;
    }
    putchar('\n');
}

/*
 * Function reads the users input and breaks it into string,
 * these strings are then stored as either a pathname or the 
 * command name for later, memset used to make sure strings
 * reset since they're global. 
*/
char* readinput(void)
{
    char *token;
    int i = 1, reset = 0;
    char line[1024];
    char *linecpy;
    printf("SamSH %% : ");
    fgets(line, sizeof(line), stdin);
    reset = strlen(line);
    linecpy = (char*)malloc(sizeof(char) * reset);
    strcpy(linecpy, line);
    strtok(linecpy, "\n");
    return linecpy;
}

char** Parse(char* input)
{
    int i = 0;
    char *token, *hold;
    char** args = (char**)malloc(sizeof(char*)*32);
    token = strtok(input, " ");
    while(token)
    {
        hold = (char*)malloc(sizeof(char)*strlen(token));
        strcpy(hold, token);
        args[i] = hold;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    i = 0;
    return args;
}

void cd(char *pathname)
{
    if(pathname)
    {
        if(chdir(pathname) == 0)
        {
            printf("cd to %s successful\n", pathname);
            return;
        }
        printf("cd to %s FAILED\n", pathname);
    }
    else
    {
        if(chdir(HOME) == 0)
        {
            printf("cd to HOME successful\n");
            return;
        }
        printf("cd to HOME FAILED\n");
    }
}


int RunProg(char *env[], char **args, char* line)
{
    int i = 1, count = 0, IOre = 0, slot = 0;
    char *hold = (char*)malloc(sizeof(char) * 32), cmd[32];
    pid_t pid = getpid(), pidchild;
    if(args[0][0] == '.' && args[0][1] == '/')
    {
        if(args[2])
        {
            args[2] = NULL;
        }
        strtok(args[0], "/");
        args[1] = strtok(NULL, "/");
        args[0] = "bash";
    }
    if(!strcmp(args[0], "cd"))
    {
        printf("Change directory\n");
        cd(args[1]);
    }
    else if(!strcmp(args[0], "exit"))
    {
        printf("SamSH PROC %d exits\n", pid);
        exit(1);
    }
    else
    {
        pidchild = fork();
        switch (pidchild)
        {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                printf("PROC %d: line=%s\n", getpid(), line);
                printf("PROC %d do_command: line=%s\n", getpid(), line);
                while(args[i])
                {
                    printf("args[%d] = %s\n", i, args[i]);
                    if(!strcmp(args[i], "|"))
                    {
                        char **head = (char**)malloc(sizeof(char*) * 32);
                        char **tail= (char**)malloc(sizeof(char*) * 32);
                        int ret = Runpipe(args, i, env, head, tail, line);
                        // free(head);
                        // free(tail);
                        // free(args);
                        return ret;
                    }
                    if(!strcmp(args[i], "<") && IOre == 0)
                    {
                        IOre = 1;
                        slot = i;
                    }
                    else if(!strcmp(args[i], ">") && IOre == 0)
                    {
                        IOre = 2;
                        slot = i;
                    }
                    else if(!strcmp(args[i], ">>") && IOre == 0)
                    {
                        IOre = 3;
                        slot = i;
                    }
                    i++;
                }
                i = 1;
                // printf("Arguments %s\n", args[0]);
                printf("PROC %d tries %s in each PATH dir\n", getpid(), args[0]);
                strcpy(cmd, args[0]);
                while(pathdirs[count])
                {
                    strcpy(hold, pathdirs[count]);
                    strcat(hold, "/");
                    strcat(hold, cmd);
                    //if(execve(hold, args, env) == -1)
                    if(access(hold, X_OK))
                    {
                        printf("Name[0]=%s i=%d cmd=%s\n", cmd, count, hold);
                    }
                    else
                    {   
                        break;
                    }
                    count++;
                }
                if(IOre != 0)
                {
                    args[slot] = NULL;
                    switch(IOre)
                    {
                        case 1:
                            close(0);
                            open(args[slot+1], O_RDONLY);
                            break;
                        case 2:
                            close(1);
                            open(args[slot+1], O_WRONLY | O_CREAT, 0664);
                            break;
                        case 3:
                            close(1);
                            open(args[slot+1], O_APPEND | O_WRONLY | O_CREAT, 0664);
                            break;
                    }
                }
                if(execve(hold, args, env) == -1)
                {
                    perror("execve");
                }
                printf("Child PROC %d died : Exit status = 0000\n", getpid());
                exit(0);
                count = 0;
            default:
                printf("Parent PROC %d forks a child process %d\n", pid, pidchild);
                printf("Parent PROC %d waits\n", pid);
                wait(0);
            }
    }
    //free(args);
}

void pipehelp(char *env[], char **args, char *line)
{
    int i = 1, count = 0, IOre = 0, slot = 0;
    char *hold = (char*)malloc(sizeof(char) * 32), cmd[32];
    pid_t pid = getpid(), pidchild;
    printf("PROC %d: line=%s\n", getpid(), line);
    printf("PROC %d do_command: line=%s\n", getpid(), line);
    while(args[i])
    {
        printf("args[%d] = %s\n", i, args[i]);
        // if(!strcmp(args[i], "|"))
        // {
        //     char **head = (char**)malloc(sizeof(char*) * 32);
        //     char **tail= (char**)malloc(sizeof(char*) * 32);
        //     return Runpipe(args, i, env, head, tail, line);
        // }
        if(!strcmp(args[i], "<") && IOre == 0)
        {
            IOre = 1;
            slot = i;
        }
        else if(!strcmp(args[i], ">") && IOre == 0)
        {
            IOre = 2;
            slot = i;
        }
        else if(!strcmp(args[i], ">>") && IOre == 0)
        {
            IOre = 3;
            slot = i;
        }
        i++;
    }
    i = 1;
    strcpy(cmd, args[0]);
    while(pathdirs[count])
    {
        strcpy(hold, pathdirs[count]);
        strcat(hold, "/");
        strcat(hold, cmd);
        if(access(hold, X_OK))
        {
            printf("Name[0]=%s i=%d cmd=%s\n", cmd, count, hold);
        }
        else
        {   
            break;
        }
        count++;
    }
    if(IOre != 0)
    {
        args[slot] = NULL;
        switch(IOre)
        {
            case 1:
                close(0);
                open(args[slot+1], O_RDONLY);
                break;
            case 2:
                close(1);
                open(args[slot+1], O_WRONLY | O_CREAT, 0664);
                break;
            case 3:
                close(1);
                open(args[slot+1], O_APPEND | O_WRONLY | O_CREAT, 0664);
                break;
        }
    }
    if(execve(hold, args, env) == -1)
    {
        perror("execve");
    }
    //exit(0);
    count = 0;
}

int Runpipe(char **args, int i, char *env[], char **head, char **tail, char* line)
{
    int n = 0, j = 0, fd[2];
    pid_t pidchild;
    for(n = 0; n < i; n++)
    {
        head[j] = args[n];
        j++;
    }
    head[j] = NULL;
    n = i+1;
    j = 0;
    while(args[n])
    {
        tail[j] = args[n];
        j++;
        n++;
    }
    tail[j] = NULL;
    pipe(fd);
    pidchild = fork();
    if(pidchild == -1)
    {
        perror("fork");
        exit(1);
        return 0;
    }
    else if(pidchild == 0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        pipehelp(env, head, line);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[1]);
        pipehelp(env, tail, line);
    }
    free(head);
    free(tail);
    free(args);
    args = NULL;
    return 0;
}