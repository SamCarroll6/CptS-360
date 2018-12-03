#include "header.h"

int symlink(void)
{
    MINODE *Path1, *Path2;
    if(name[0] && paths[1])
    {
        if(!strcmp(name[0], "/"))
        {
            Path1 = findval(root);
        }
        else
        {
            Path1 = findval(running->cwd);
        }
        INODE *check = &Path1->INODE;
        if(!S_ISREG(check->i_mode) && !S_ISDIR(check->i_mode))
        {
            printf("Error: file1 must be of type directory or regular file.\n");
            return -1;
        }
        // Filename length can't be longer than 60.
        if(strlen(paths[0]) >= 60)
        {
            printf("Error: file1 name too large symlnk failed\n");
            return -1;
        }
        // change name[] from file1 to file2
        tokenize(paths[1]);
        if(creat_file() == -1)
        {
            printf("Error: symlink failed\n");
            return -1;
        }
        // creat_file overwrites name[] so we'll
        // recreate it again.
        tokenize(paths[1]);
        if(!strcmp(name[0], "/"))
        {
            Path2 = findval(root);
        }
        else
        {
            Path2 = findval(running->cwd);
        }
        INODE *pip = &Path2->INODE;
        pip->i_mode = 0120000;
        // if the strlen of the path is less than 60
        // store the name because it fits 
        // according to KC you have room for 60.
        strcpy(pip->i_block, paths[0]);
        printf("%s\n", (char*)pip->i_block);
        pip->i_size = strlen(paths[0]);
        Path2->dirty = 1;
        iput(Path2);
        // if not just store the size for read link to
        // use.
        return 1;
    }
    printf("Usage: symlink file1 file2\n");
    return -1;
}