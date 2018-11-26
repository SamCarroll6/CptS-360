#include "header.h"

/*
    Helper function, just returns if a file 
    type is directory or standard file.
*/
int checktype(MINODE *mip)
{
    if(!mip)
        return 0;
    INODE* pip = &mip->INODE;
    u16 mode = pip->i_mode;
    u16 type = mode & 0xF000;
    switch(type)
    {
        case 0x4000:
            return 1;
        default:
            return 0;
    }
}

/*
    Traverses through our directory mip, each file contained within
    mip is passed to ls_file to print it's contents.
*/
void ls_dir(MINODE *mip)
{
    INODE *pip = &mip->INODE;
    int i = 0;
    int size, count = 0;
    int buf[BLKSIZE];
    size = pip->i_size;
    for(i; i < 12; i++)
    {
        if(pip->i_block[i] == 0)
        {
            break;
        }
        printf("i_block[%d] = %d\n", i, pip->i_block[i]);
        get_block(mip->dev, pip->i_block[i], buf);
        dp = (DIR *)buf;
        char nameval[BLKSIZE + 1];
        while(count < size && dp->inode)
        {
            memcpy(nameval, dp->name, dp->name_len);
            nameval[dp->name_len] = '\0';
            MINODE* next = iget(mip->dev, dp->inode);
            ls_file(next, nameval);
            count+=dp->rec_len;
            dp = (void *)dp + dp->rec_len;
        }
    }
}

/*
    Prints like ls -l in standard linux. This
    function handles printing the information for
    an individual file or directory that is passed 
    in via MINODE* mip.
*/
void ls_file(MINODE* mip, char *name2)
{
    INODE* pip = &mip->INODE;
    u16 mode = pip->i_mode;
    time_t val = pip->i_ctime;
    char *mtime = ctime(&val);
    mtime[strlen(mtime) - 1] = '\0';
    //printf("0x%x\n", (mode & 0xF000));
    u16 type = mode & 0xF000;
    switch(type)
    {
        case 0x4000:
            printf("d");
            break;
        case 0x8000:
            printf("-");
            break;
        case 0xA000:
            printf("l");
            break;
        default:
            printf("-");
            break;
    }
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
    printf("%4d%4d%4d  %s%8d    %s\n", pip->i_links_count, pip->i_gid, pip->i_uid, mtime, pip->i_size, name2);
}

/*
    Follow tokenized pathname using search function.
    If path exists you'll have the ino of the directory
    returned to you, turn this to MINODE and return.
    If directory doesn't exist return NULL. 
*/
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
    while(name[i])
    {
        ino = search(check, name[i]);
        if(ino == -1)
        {
            printf("Name %s does not exist\n", name[i]);
            return NULL;
        }
        mip = iget(fd, ino);
        check = &mip->INODE;
        i++;
    }
    return mip;
}

int chdir(void)
{
    MINODE *pathfollow;
    if(name[0] == NULL)
    {
        iput(running->cwd);
        running->cwd = root;
    }
    else
    {
        if(!strcmp(name[0], "/"))
        {
            pathfollow = findval(root);
        }
        else
        {
            pathfollow = findval(running->cwd);
        }
        if(checktype(pathfollow))
        {
            iput(running->cwd);
            running->cwd = pathfollow;
        }
        else
        {
            int i = 0;
            while(name[i])
            {
                i++;
            }
            printf("Error: %s is not a directory\n", name[i - 1]);
        }
    }
}

int findmyname(MINODE *parent, u32 myino, char **myname)
{
  INODE *pip = &parent->INODE;
  int size, count = 0;
  char buf[BLKSIZE];
  size = pip->i_size;
  get_block(fd, pip->i_block[0], buf);

  dp = (DIR *)buf;
  while(count < size && dp->inode)
  {
	if(dp->inode == myino)
	{
        *myname = (char*)malloc(sizeof(char) * (dp->name_len + 1));
        strncpy(*myname, dp->name, dp->name_len);
        (*myname)[dp->name_len] = '\0';
		return 0;
	}
        dp = (void *)dp + dp->rec_len;
        count+=dp->rec_len;
  }
  return 1;
}

int findino(MINODE *mip, u32 myino)
{
    return getino(mip, "..");
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
        putchar('\n');
    }
}

void rpwd(MINODE *pr)
{
    MINODE *pip;
    char *name2;
    if(pr == root)
    {
        //printf("/");
        return;
    }
    int pino = findino(pr, pr->ino);
    pip = iget(pr->dev, pino);
    findmyname(pip, pr->ino, &name2);
    rpwd(pip);
    printf("/%s", name2);
}

int quit(void)
{
    int i;
    MINODE *mip;
    for(i = 0; i < NMINODE; i++)
    {
        mip = &minode[i];
        if(mip->refCount > 0 && mip->dirty)
            iput(mip);
    }
    exit(0);
}