#include "header.h"

int mdir(void)
{
    int i = 0;
    MINODE *parent;
    if(name[0] == NULL)
    {    
        printf("Usage: mkdir requires pathname\n");
        return -1;
    }
    while(name[i])
    {
        i++;
    }
    i = i - 1;
    char bname[64];
    strcpy(bname, name[i]);
    name[i] = NULL;
    if(!strcmp(bname, "/"))
    {
        printf("mkdir: cannot create directory '/': File exists\n");
        return -1;
    }
    if(!strcmp(name[0], "/"))
        parent = findval(root);
    else
    {
        parent = findval(running->cwd);
    }
    if(checktype(parent))
    {
        INODE *pip = &parent->INODE;
        if(search2(pip, bname) == -1)
        {
            smkdir(parent, bname);
        }
        printf("mkdir: cannot create directory '%s': File exists\n", bname);
    }
    printf("Path must be of type directory\n");
    return -1;
}

int smkdir(MINODE *mip, char *bname)
{
    int ino = ialloc(mip->dev);
    int block = balloc(mip->dev);
    printf("%d %d\n", ino, block);
    if(ino && block)
    {
        MINODE *new = iget(mip->dev, ino);
        // 004 => directory, 755 => permissions
        new->INODE.i_mode = 0040755;
        new->INODE.i_block[0] = block;
        printf("iblock = %d\n", new->INODE.i_block[1]);
    }
    printf("mkdir: cannot create directory '%s': No available inodes/blocks\n", bname);
}