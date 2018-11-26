#include "header.h"

void ls_dir(MINODE *mip)
{
    ip = &mip->INODE;
    int i = 0;
    int size, count = 0;
    char buf[BLKSIZE];
    size = ip->i_size;
    for(i; i < 12; i++)
    {
        if(ip->i_block[i] == 0)
        {
            break;
        }
        get_block(fd, ip->i_block[i], buf);
        dp = (DIR *)buf;
        char nameval[BLKSIZE + 1];
        while(count < size && dp->inode)
        {
            memcpy(nameval, dp->name, dp->name_len);
            nameval[dp->name_len] = '\0';
            MINODE* next = iget(fd, dp->inode);
            ls_file(next, nameval);
            count+=dp->rec_len;
            dp = (void *)dp + dp->rec_len;
        }
    }
}

void ls_file(MINODE* mip, char *name2)
{
    INODE* place = &mip->INODE;
    printf("Mode = %x\n", place->i_mode);
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