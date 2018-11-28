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
            return smkdir(parent, bname);
        }
        printf("mkdir: cannot create directory '%s': File exists\n", bname);
        return -1;
    }
    printf("Path must be of type directory\n");
    return -1;
}

int smkdir(MINODE *mip, char *bname)
{
    int ino = ialloc(mip->dev), i;
    int block = balloc(mip->dev);
    char buf[BLKSIZE];
    char *cp;
    DIR *ddp;
    if(ino != 0 && block != 0)
    {
        MINODE *new = iget(mip->dev, ino);
        INODE *pip = &(new->INODE), *PMip = &(mip->INODE);
        // 004 => directory, 755 => permissions
        // Write all the information to new inode
        pip->i_mode = 0040755;
        pip->i_block[0] = block;
        for(i = 1; i < 12; i++)
        {
            pip->i_block[i] = 0;
        }
        pip->i_uid = running->uid;
        pip->i_gid = running->gid;
        pip->i_size = BLKSIZE;
        pip->i_links_count = 2;
        pip->i_atime = time(NULL);
        pip->i_ctime = time(NULL);
        pip->i_mtime = time(NULL);
        pip->i_blocks = 2;
        // Change new to dirty and write to disk
        new->dirty = 1;
        iput(new);
        // Adjust parent node, add a link due to new
        // dir and make dirty, then write to disk.
        PMip->i_links_count++;
        mip->dirty = 1;
        iput(mip);

        get_block(new->dev, pip->i_block[0], buf);
        ddp = (DIR*)buf;
        cp = buf;
        ddp->inode = ino;
        ddp->file_type = EXT2_FT_DIR;
        // Ideal rec length, this is for all except last entry
        // Last entry makes distance to BLKSIZE.
        ddp->rec_len = 4 * ((8 + 1 + 3) / 4);
        ddp->name_len = strlen(".");
        ddp->name[0] = '.';

        cp += ddp->rec_len;
        ddp = (DIR*)cp;
        
        ddp->inode = mip->ino;
        // BLKSIZE - previous directories record length
        ddp->rec_len = BLKSIZE - 12;
        ddp->file_type = EXT2_FT_DIR;
        dp->name_len = strlen("..");
        dp->name[0] = '.';
        dp->name[1] = '.';
        printf("Write block\n");
        // Write block back to disk block
        put_block(new->dev, block, buf);
        enter_child(mip, ino, bname);

        return 1;
    }
    printf("mkdir: cannot create directory '%s': No available inodes/blocks\n", bname);
    return -1;
}


int enter_child(MINODE *mip, int ino, char *bname)
{
    int i = 0;
    INODE *Pinode = &mip->INODE;
    char buf[BLKSIZE];
    char *cp;

    // for(i; i < Pinode->i_size; i++)
    // {

    // }
}