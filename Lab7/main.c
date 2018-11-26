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
        printf("%s\n", input);
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
                    printf("pathfollow = %d\n", pathfollow->ino);
                    ls_dir(pathfollow);
                }
            }
            else if(!strcmp(input, "cd"))
            {

            }
            else if(!strcmp(input, "pwd"))
            {
                pwd(root);
            }
            else if(!strcmp(input, "quit"))
            {

            }
            else
            {
                printf("Error: Invalid command %s\n", input);
            }
        // while(name[i])
        // {
        //     printf("%s\n", name[i]);
        //     i++;
        // }
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
