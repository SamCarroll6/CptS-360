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
    if(!strcmp(bname, "/"))
    {
        printf("rmdir: cannot remove '/': cannot remove root\n");
        return -1;
    }
    if(path == NULL)
    {
        printf("rmdir: cannot remove directory '%s': File does not exists\n", bname);
        return -1;
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
    int size;
    char buf[BLKSIZE];
    char *cp;
    size = pip->i_size;
    get_block(fd, pip->i_block[0], buf);
    dp = (DIR *)buf;
    cp = buf;
    char nameval[BLKSIZE + 1];
    while(cp < &buf[BLKSIZE])
    {
        memcpy(nameval, dp->name, dp->name_len);
        nameval[dp->name_len] = '\0';
	    if(!strcmp(nameval, name))
	    {
		    
	    }
        cp += dp->rec_len;
        dp = (DIR*)cp;
    }
    return -1;
}