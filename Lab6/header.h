#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <libgen.h>

typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR; 

char *Bname;
char *path[25];

int splitpath(char *pathname)
{
    char *hold = (char*)malloc(sizeof(char) * strlen(pathname));
    Bname = basename(pathname);
    strcpy(hold, pathname);
    pathname = dirname(hold);
    path[0] = strtok(pathname, "/");
    int i = 1;
    while(path[i] = strtok(NULL, "/"))
    {
        i++;
    }
    printf("Basename = %s\n", Bname);
    i = 0;
    while(path[i])
    {
        printf("path[%d] = %s\n", i, path[i]);
        i++;
    }
}