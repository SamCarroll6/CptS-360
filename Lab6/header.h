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

#define BLKSIZE 1024

GD    *gp;
SUPER *sp;
INODE *ip;
DIR *dp; 

int fd;
int iblock;

char *Bname;
char *path[25];

int splitpath(char *pathname, int dev)
{
    char *hold = (char*)malloc(sizeof(char) * strlen(pathname));
    fd = dev;
    Bname = basename(pathname);
    if(!strcmp(Bname, "/"))
    {
        Bname = ".";
    }
    strcpy(hold, pathname);
    pathname = dirname(hold);
    path[0] = strtok(pathname, "/");
    int i = 2;
    while(path[i] = strtok(NULL, "/"))
    {
        i++;
    }
}

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd,(long)blk*BLKSIZE, 0);
   read(fd, buf, BLKSIZE);
}

int search(INODE *ip, char *name)
{
  int size, count = 0;
  char buf[BLKSIZE];
  size = ip->i_size;

  get_block(fd, ip->i_block[0], buf);

  dp = (DIR *)buf;
  char nameval[BLKSIZE + 1];
  while(count < size && dp->inode)
  {
    memcpy(nameval, dp->name, dp->name_len);
    nameval[dp->name_len] = '\0';
	if(!strcmp(nameval, name))
	{
		return dp->inode;	
	}
        dp = (void *)dp + dp->rec_len;
        count+=dp->rec_len;
  }
  return -1;
}

int getInode(void)
{
  char buf[BLKSIZE];
  int size, count = 0;
  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  
  iblock = gp->bg_inode_table;   // get inode start block#
  printf("inode_block=%d\n", iblock);

  // get inode start block     
  get_block(fd, iblock, buf);
  
  ip = (INODE *)buf + 1;  // ip points at 2nd INODE
  size = ip->i_size;
  int i = 0;
  int ret = 0, blk, offset;
  while(path[i])
  {
    ret = search(ip, path[i]);
    if(ret == -1)
    {
        printf("can't find %s from pathname\n", path[i]);
        exit(1);
    }
    blk = (ret - 1) / 8 + iblock;  // disk block contain this INODE 
    offset = (ret - 1) % 8;         // offset of INODE in this block
    get_block(fd, blk, buf);
    ip = (INODE *)buf + offset;    // ip -> new INODE
    i++;
  }
  if(Bname)
  {
    ret = search(ip, Bname);
    if(ret == -1)
    {
      printf("can't find %s from pathname\n", path[i]);
      exit(1);
    }
    blk = (ret - 1) / 8 + iblock;  // disk block contain this INODE 
    offset = (ret - 1) % 8;         // offset of INODE in this block
    get_block(fd, blk, buf);
    ip = (INODE *)buf + offset;    // ip -> new INODE
    i = 0;
  }
  for(i;i < 12;i++)
  {
      printf("[%d]%d  ", i, ip->i_block[i]);
  }
  printf("\n");
  dp = (DIR*)buf;

  char nameval[BLKSIZE + 1];

  while(count < size && dp->inode)
  {
  	strncpy(nameval, dp->name, dp->name_len);
	nameval[dp->name_len] = '\0';
  	printf("%10u\t%s\n", dp->inode, nameval);
    dp = (void *)dp + dp->rec_len;
	count+=dp->rec_len;	
  }
}