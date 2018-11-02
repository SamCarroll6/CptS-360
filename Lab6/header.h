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
    int i = 1;
    while(path[i] = strtok(NULL, "/"))
    {
        i++;
    }
    // printf("Bname = %s\n", Bname);
    // i = 0;
    // while(path[i])
    // {
    //     printf("path[%d] = %s\n", i, path[i]);
    //     i++;
    // }
}

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd,(long)blk*BLKSIZE, 0);
   read(fd, buf, BLKSIZE);
}

int search(INODE *ip, char *name)
{
  printf("Search for %s\n", name);
  printf("i_block[0] = %d\n", ip->i_block[0]);
  int size, count = 0;
  char buf[BLKSIZE];
  size = ip->i_size;

  get_block(fd, ip->i_block[0], buf);

  dp = (DIR *)buf;
  char nameval[BLKSIZE + 1];
  printf("i_number  rec_len    name_len   name\n");
  while(count < size && dp->inode)
  {
    memcpy(nameval, dp->name, dp->name_len);
    nameval[dp->name_len] = '\0';
    printf("%d\t  %d\t\t%d\t%s\n", dp->inode, dp->rec_len, dp->name_len, dp->name);
	if(!strcmp(nameval, name))
	{
		return dp->inode;	
	}
        dp = (void *)dp + dp->rec_len;
        count+=dp->rec_len;
  }
  return -1;
}

void printindirects(int size, char buf[])
{
    int i = 0;
    for(i; i < size; i++)
    {
        printf("[%d]%d  ", i, buf[i]);
        if((i + 1)%12 == 0)
        {
            putchar('\n');
        }
    }
}

int getInode(void)
{
  char buf[BLKSIZE];
  int size, count = 12;
  get_block(fd, 1, buf);
  sp = (SUPER*)buf;
  if(sp->s_magic != 0xEF53)
  {
      printf("File is not an ext2 fs\n");
      exit(1);
  }
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
    printf("===========================================\n");
    printf("i = %d name = %s", i, path[i]);
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
    printf("===========================================\n");
    printf("i = %d name = %s", i, Bname);
    ret = search(ip, Bname);
    if(ret == -1)
    {
      printf("can't find %s from pathname\n", Bname);
      exit(1);
    }
    blk = (ret - 1) / 8 + iblock;  // disk block contain this INODE 
    offset = (ret - 1) % 8;         // offset of INODE in this block
    get_block(fd, blk, buf);
    ip = (INODE *)buf + offset;    // ip -> new INODE
    i = 0;
  }
  printf("***************** First 12 blocks ******************\n");
  for(i;i < 12;i++)
  {
      printf("[%d]%d  ", i, ip->i_block[i]);
  }
  printf("\n");

  printf("***************** Indirect Blocks ******************\n");
  get_block(fd, ip->i_block[12], buf);
  printindirects(256, buf);
  putchar('\n');
  printf("************** Double Indirect Blocks **************\n");
  get_block(fd, ip->i_block[13], buf);
  char hold[BLKSIZE];
  for(i = 0; i < 256; i++)
  {
      if(buf[i] == 0)
      {
          break;
      }
      get_block(fd, buf[i], hold);
      printindirects(256, hold);
  }

}