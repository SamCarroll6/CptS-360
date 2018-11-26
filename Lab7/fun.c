#include "header.h"

int get_block(int fd, int blk, char buf[])
{
    lseek(fd, (long)blk*BLKSIZE, 0);
    read(fd, buf, BLKSIZE);
}

int put_block(int fd, int blk, char buf[])
{
    lseek(fd, (long)blk*BLKSIZE, 0);
    write(fd, buf, BLKSIZE);
}

int mountroot(char *diskname)
{
    printf("mountroot()\n");
    char buf[BLKSIZE];
    get_block(fd, 1, buf);
    sp = (SUPER*)buf;
    if(sp->s_magic != 0xEF53)
    {
        printf("Magic = %x is not an ext2 filesystem\n", sp->s_magic);
        exit(1);
    }
    nblocks = sp->s_blocks_count;
    ninodes = sp->s_inodes_count;
    get_block(fd, 2, buf);
    gp = (GD*)buf;
    bmap = gp->bg_block_bitmap; 
    imap = gp->bg_inode_bitmap; 
    inode_start = gp->bg_inode_table;
    root = iget(fd, 2);
    proc[0].cwd = iget(fd, 2);
    proc[1].cwd = iget(fd, 2);
    running = &proc[0];
}

int init(void)
{
    printf("Checking EXT2 FS... init()\n");
    int i = 0;
    int count = 0;
    for(i = 0; i < NPROC; i++)
    {
        proc[i].pid = i;
        proc[i].uid = i;
        for(count = 0; count < NFD; count++)
        {
            proc[i].fd[count] = 0;
        }
        proc[i].next = &proc[i+1];
    }
    proc[NPROC - 1].next = &proc[0];
    for(i; i < NMINODE; i++)
    {
        minode[i].refCount = 0;
    }
    running = &proc[0];
    root = NULL;
}

MINODE *iget(int dev, int ino)
{
    MINODE *mip;
    int i = 0;
    for(i; i < NMINODE; i++)
    {
        mip = &minode[i];
        if((mip->refCount) && (mip->dev == dev) && (mip->ino == ino))
        {
            mip->refCount++;
            return mip;
        }
    }
    for(i = 0; i < NMINODE; i++)
    {
        mip = &minode[i];
        if(mip->refCount == 0)
        {
            mip->dev = dev;
            mip->ino = ino;
            mip->refCount++;
            mip->dirty = 0;
            mip->mounted = 0;
            mip->INODE = *get_inode(dev, ino);
            return mip;
        }
    }
    printf("Error: No MINODES\n");
    return 0;
}

int iput(MINODE *mip)
{
    char buf[BLKSIZE];
    INODE *pip;
    if(mip == NULL)
        return;
    mip->refCount--;
    if(mip->refCount > 0)
        return;
    if(!mip->dirty)
        return;
    int ino = getino(mip, ".");
    // 8 is the number of inodes per block
    int blk = (ino - 1) / 8 + inode_start;
    int offset = (ino - 1) % 8;
    get_block(dev, blk, buf);
    pip = (INODE*)buf + offset;
    *pip = mip->INODE;
    put_block(mip->dev, blk, buf);
    mip->dirty = 0;
}

int getino(MINODE *mip, char *name2)
{
    int i, ino;
    INODE *check = &mip->INODE;
    ino = search2(check, name2);
    if(ino == -1)
    {
        printf("Name %s not found\n", name2);
        return -1;
    }
    return ino;
}

INODE *get_inode(int dev, int ino)
{
    char buf[BLKSIZE];
    int blk, offset;
    get_block(dev, inode_start, buf);
    ip = (INODE*)buf + 1;
    blk = (ino - 1) / 8 + inode_start;
    offset = (ino - 1) % 8;
    get_block(dev, blk, buf);
    ip = (INODE*)buf + offset;
    return ip;
}

char* readinput(void)
{
    char *token;
    int i = 1, reset = 0;
    char line[1024];
    char *linecpy;
    printf("input command : [ls|cd|pwd|quit] ");
    fgets(line, sizeof(line), stdin);
    reset = strlen(line);
    linecpy = (char*)malloc(sizeof(char) * reset);
    strcpy(linecpy, line);
    strtok(linecpy, "\n");
    return linecpy;
}

char* Parse(char* input)
{
    int i = 0;
    char *token, *hold;
    token = strtok(input, " ");
    printf("input = %s\n", input);
    if(token = strtok(NULL, " "))
    {
        return token;
    }
    return NULL;
}

int tokenize(char *pathname)
{
    printf("path = %s\n", pathname);
    if(pathname == NULL)
        return 0;
    char *token;
    int i = 0;
    if(pathname[0] == '/')
    {
        name[i] = "/";
        i++;
    }
    token = strtok(pathname, "/");
    name[i] = token;
    i++;
    while(token)
    {
        token = strtok(NULL, "/");
        if(token)
        {
            name[i] = token;
            i++;
        }
    }
    return i;
}

int search(INODE *ip, char *name)
{
  printf("Search for %s\n", name);
  printf("i_block[0] = %d\n", ip->i_block[0]);
  int size, count = 0;
  char buf[BLKSIZE];
  size = ip->i_size;
  printf("%d\n", size);
  get_block(fd, ip->i_block[0], buf);

  dp = (DIR *)buf;
  char nameval[BLKSIZE + 1];
  while(count < size && dp->inode)
  {
    memcpy(nameval, dp->name, dp->name_len);
    nameval[dp->name_len] = '\0';
    printf("%d\t  %d\t\t%d\t%s\n", dp->inode, dp->rec_len, dp->name_len, dp->name);
	if(!strcmp(nameval, name))
	{
        printf("===========================================\n");
		return dp->inode;
	}
        dp = (void *)dp + dp->rec_len;
        count+=dp->rec_len;
  }
  printf("===========================================\n");
  return -1;
}

int search2(INODE *ip, char *name)
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