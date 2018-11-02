#include "header.h"

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: DiskName Pathname\n");
        return 0;
    }
    char *disk = argv[1];
    int fd = open(disk, O_RDONLY);
    if(fd<0)
    {
        printf("File %s could not be opened\n", disk);
        exit(1);
    }
    char *pathname = argv[2];
    splitpath(pathname, fd);
    getInode();
}