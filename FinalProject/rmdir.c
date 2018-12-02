#include "header.h"

int rm_dir(void)
{
    int i = 0;
    MINODE *path, *parent;
    if(name[0] == NULL)
    {    
        printf("Usage: rmdir requires pathname\n");
        return -1;
    }
    // Get MINODE of pathname
    if(!strcmp(name[0], "/"))
    {
        path = findval(root);
    }
    else
    {
        path = findval(running->cwd);
    }
    while(name[i])
    {
        i++;
    }
    i = i - 1;
    char bname[64];
    strcpy(bname, name[i]);
    name[i] = NULL;
    if(path == root)
    {
        printf("rmdir: cannot remove '%s': cannot remove root\n", bname);
        return -1;
    }
    if(path == NULL)
    {
        printf("rmdir: cannot remove directory '%s': File does not exists\n", bname);
        return -1;
    }
    if(path == running->cwd)
    {
        printf("Error: cannot remove current working directory\n");
        return -1;
    }
    // If the bname is '.' for whatever reason
    // we need to ensure the name is stored correctly
    // we don't want to delete '.' from itself, we want to delete
    // '.' from it's actual parent.
    if(!strcmp(bname, "."))
    {
        strcpy(bname, name[i-1]);
        name[i-1] == NULL;
    }
    // Get MINODE of parent to path
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
    if(checktype(path) && is_empty(path))
    {
        INODE *pip = &parent->INODE;
        int ret = rm_child(parent, bname);
        return ret;
    }
    printf("Path must be of type directory and must be empty\n");
    return -1;
}


int rm_child(MINODE *pmip, char *name)
{
    INODE *pip = &pmip->INODE;
    int size, i;
    char buf[BLKSIZE];
    char *cp;
    DIR *prev;
    size = pip->i_size;
    for(i = 0; i < 12; i++)
    {
        if(pip->i_block[i] == 0)
        {
            printf("Error: Bname not found\n");
            return -1;
        }
        get_block(pmip->dev, pip->i_block[i], buf);
        dp = (DIR *)buf;
        cp = buf;
        char nameval[BLKSIZE + 1];
        while(cp < &buf[BLKSIZE])
        {
            memcpy(nameval, dp->name, dp->name_len);
            nameval[dp->name_len] = '\0';
	        if(!strcmp(nameval, name))
	        {
                // if it is first and only entry in a block
                // check if cp == buf (first entry in block)
                // and if cp + dp->rec_len = the end of the block.
                if(cp == buf && cp + dp->rec_len == &buf[BLKSIZE])
                {
                    // deallocate block
                    bdalloc(pmip->dev, pip->i_block[i]);
                    // decrement parent size
                    pip->i_size -= BLKSIZE;
                    int count = i + 1;
                    // Traverse remaining block and move down
                    // one spot in i_block[i] to remove blanks.
                    for(count; count < 12; count++)
                    {
                        if(pip->i_block[count])
                        {
                            get_block(pmip->dev, pip->i_block[count], buf);
                            put_block(pmip->dev, pip->i_block[count], buf);
                        }
                        else
                        {
                            printf("Directory successfully removed\n");
                            break;
                        }
                    }
                }
                // if it is last entry in a block remove it and give previous 
                // reclen it's reclen so total is still BLKSIZE.
                else if(cp + dp->rec_len == &buf[BLKSIZE])
                {
                    prev->rec_len += dp->rec_len;
                    put_block(pmip->dev, pip->i_block[i], buf);
                }
                // dp is just an entry in the middle somewhere,
                // or start of a block with other records.
                else
                {
                    DIR *find_end = (DIR*)buf;
                    char *final_cp = buf;
                    // While loop to get the last entry in block.
                    while(final_cp + find_end->rec_len < &buf[BLKSIZE])
                    {
                        final_cp += find_end->rec_len;
                        find_end = (DIR*)final_cp;
                    }
                    find_end->rec_len += dp->rec_len;
                    // Move all records to the left to ensure
                    // data still fits.
                    char *moveS = cp + dp->rec_len;
                    char *moveE = buf + BLKSIZE;
                    // C function to move memory to the left.
                    memmove(cp, moveS, moveE-moveS);
                    put_block(pmip->dev, pip->i_block[i], buf);
                }
                return 1;
	        }
            prev = dp;
            cp += dp->rec_len;
            dp = (DIR*)cp;
        }
        pmip->dirty = 1;
        iput(pmip);
        return 1;
    }
    return -1;
}