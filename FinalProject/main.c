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
                lsdo();
            }
            else if(!strcmp(input, "cd"))
            {
                chdir();
            }
            else if(!strcmp(input, "pwd"))
            {
                pwd();
            }
            else if(!strcmp(input, "quit"))
            {
                quit();
            }
            else if(!strcmp(input, "mkdir"))
            {
                mdir();
            }
            else if(!strcmp(input, "creat"))
            {
                creat_file();
            }
            else if(!strcmp(input, "rmdir"))
            {
                rm_dir();
            }
            else if(!strcmp(input, "symlink"))
            {
                symlink();
            }
            else
            {
                printf("Error: Invalid command %s\n", input);
            }
        }
        // i = 0;
        // while(name[i])
        // {
        //     name[i] = NULL;
        //     i++;
        // }
        i = 0;
        while(paths[i])
        {
            free(paths[i]);
            paths[i] = NULL;
            i++;
        }
        input = NULL;
        hold = NULL;
    }
}
