#include <stdio.h>       // for printf()
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcpy(), strcmp(), etc.
#include <libgen.h>      // for basename(), dirname()
#include <fcntl.h>       // for open(), close(), read(), write()

// for stat syscalls
#include <sys/stat.h>
#include <unistd.h>

// for opendir, readdir syscalls
#include <sys/types.h>
#include <dirent.h>

int myrcp(char *f1, char *f2)
{
   1. stat f1;   if f1 does not exist ==> exit.
                 f1 exists: reject if f1 is not REG or LNK file
   2. stat f2;   reject if f2 exists and is not REG or LNK file

   3. if (f1 is REG){
         if (f2 non-exist OR exists and is REG)
            return cpf2f(f1, f2);
	 else // f2 exist and is DIR
            return cpf2d(f1,f2);
      }
   4. if (f1 is DIR){
	if (f2 exists but not DIR) ==> reject;
        if (f2 does not exist)     ==> mkdir f2
	return cpd2d(f1, f2);
      }
}

// cp file to file
int cpf2f(char *f1, char *f2)
{
  1. reject if f1 and f2 are the SAME file
  2. if f1 is LNK and f2 exists: reject
  3. if f1 is LNK and f2 does not exist: create LNK file f2 SAME as f1
  4:
     open f1 for READ;
     open f2 for O_WRONLY|O_CREAT|O_TRUNC, mode=mode_of_f1;
     copy f1 to f2
}

int cpf2d(char *f1, char *f2)
{
  1. search DIR f2 for basename(f1)
     (use opendir(), readdir())
     // x=basename(f1); 
     // if x not in f2/ ==>        cpf2f(f1, f2/x)
     // if x already in f2/: 
     //      if f2/x is a file ==> cpf2f(f1, f2/x)
     //      if f2/x is a DIR  ==> cpf2d(f1, f2/x)

}



int cpd2d(char *f1, char *f2)
{
    // recursively cp dir into dir    
}