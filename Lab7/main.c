#include "header.h"

int main(int argc, char *argv[])
{
    char *diskname = "mydisk";
    char *input, *hold, **path;
    if(argc > 1)
        diskname = argv[1];
    fd = open(diskname, O_RDWR);
    if(fd < 0)
    {
        printf("Checking EXT2 FS... Open %s failed\n", diskname);
        exit(1);
    }
    init();
    mountroot(diskname);
    int ret;
    MINODE* pathfollow;
    while(1)
    {
        input = readinput();
        hold = Parse(input);
        ret = tokenize(hold);
        int i = 0;
        if(input != 0)
        {
            if(!strcmp(input, "ls"))
            {
                if(name[0] == NULL)
                {
                    ls_dir(running->cwd);
                }
                else
                {
                    if(strcmp(name[0], "/"))
                        pathfollow = findval(root);
                    else
                    {
                        pathfollow = findval(running->cwd);
                    }
                    if(pathfollow != NULL)
                    {
                        if(checktype(pathfollow))
                            ls_dir(pathfollow);
                        else
                        {
                            while(name[i])
                            {
                                i++;
                            }
                            ls_file(pathfollow, name[i - 1]);
                        }
                    }
                }
            }
            else if(!strcmp(input, "cd"))
            {
                chdir();
            }
            else if(!strcmp(input, "pwd"))
            {
                pwd(running->cwd);
            }
            else if(!strcmp(input, "quit"))
            {

            }
            else
            {
                printf("Error: Invalid command %s\n", input);
            }
        }
        i = 0;
        while(name[i])
        {
            name[i] = NULL;
            i++;
        }
        input = NULL;
        hold = NULL;
    }
}
