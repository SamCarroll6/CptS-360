#include "header.h"

int get_block(int fd, int blk, char buf[])
{
    lseek(fd, (long)blk*BLKSIZE, 0);
    read(fd, buf, BLKSIZE);
}

int mountroot(char *diskname)
{
    char buf[BLKSIZE];
    get_block(fd, 1, buf);
    sp = (SUPER*)buf;
    if(sp->s_magic != 0xEF53)
    {
        printf("Magic = %x is not an ext2 filesystem\n", sp->s_magic);
        exit(1);
    }
}

init()
{

}

mountroot()
{

}
