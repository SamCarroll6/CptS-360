#include "header.h"

int creat_file(void)
{
    int i = 0;
    MINODE *parent;
    if(name[0] == NULL)
    {    
        printf("Usage: creat requires pathname\n");
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
        printf("creat: cannot create file '/': File exists\n");
        return -1;
    }
    if(name[0] == NULL)
    {
        parent = findval(running->cwd);
    }
    else if(!strcmp(name[0], "/"))
    {
        parent = findval(root);
    }
    else
    {
        parent = findval(running->cwd);
    }
    if(checktype(parent))
    {
        INODE *pip = &parent->INODE;
        if(search2(pip, bname) == -1)
        {
            int ret = mycreat(parent, bname);
            pip->i_links_count++;
            parent->dirty = 1;
            iput(parent);
            return ret;
        }
        printf("creat: cannot create file '%s': File exists\n", bname);
        return -1;
    }
    printf("Path must be of type directory\n");
    return -1;
}

int mycreat(MINODE *mip, char *bname)
{
    int ino = ialloc(mip->dev), i;
    char buf[BLKSIZE];
    char *cp;
    DIR *ddp;
    if(ino != 0)
    {
        MINODE *new = iget(mip->dev, ino);
        INODE *pip = &new->INODE, *PMip = &mip->INODE;
        // 0x81A4 is type file.
        // Write all the information to new inode
        pip->i_mode = 0x81A4;
        for(i = 0; i < 12; i++)
        {
            pip->i_block[i] = 0;
        }
        pip->i_uid = running->uid;
        pip->i_gid = running->gid;
        pip->i_size = 0;
        pip->i_links_count = 1;
        pip->i_atime = time(NULL);
        pip->i_ctime = time(NULL);
        pip->i_mtime = time(NULL);
        pip->i_blocks = 0;
        // Change new to dirty and write to disk
        new->dirty = 1;
        iput(new);

        enter_child(mip, ino, bname);

        return 1;
    }
    printf("creat: cannot create file '%s': No available inodes\n", bname);
    return -1;
}
