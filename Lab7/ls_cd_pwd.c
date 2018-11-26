#include "header.h"

void ls_dir(MINODE *mip)
{

}

void ls_file(MINODE* mip, char *name2)
{
    
}

MINODE* findval(MINODE *mip)
{
    int i, ino;
    INODE *check = &mip->INODE;
    if(!strcmp(name[0],"/"))
        i = 1;
    else
    {
        i = 0;
    }
    printf("i = %d\n", i);
    while(name[i])
    {
        ino = search(check, name[i]);
        mip = iget(fd, ino);
        check = &mip->INODE;
        i++;
    }
    return mip;
}

void pwd(MINODE *pr)
{
    if(pr == root)
    {
        printf("/\n");
    }
    else
    {
        rpwd(pr);
    }
}

void rpwd(MINODE *pr)
{
    MINODE *pip;
    if(pr == root)
    {
        return;
    }

}