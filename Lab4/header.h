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
   struct stat buf1;
   struct stat buf2;
   int f2exist = stat(f2, &buf2); // 0 if it exists, 1 if not
   //if f1 does not exist ==> exit.
   if(stat(f1, &buf1))
   {
         printf("Usage: rcp SRC DST\n");
         return 0;
   }
   //f1 exists: reject if f1 is not REG or LNK file
   if(!S_ISREG(buf1.st_mode) && !S_ISLNK(buf1.st_mode)) 
   {
         printf("%s is not a file; try dir\n", f1);
         if (S_ISDIR(buf1.st_mode))
         {
            if (!f2exist && !S_ISDIR(buf2.st_mode))
            {
                  printf("Can't cp dir into file\n");
                  return 0;
            }
            if (f2exist)
            {
                  mkdir(f2, 755);
            }
	      return cpd2d(f1, f2);
         }
         printf("%s file type not recognized\n", f1);
         return 0;
   }  
   //reject if f2 exists and is not REG or LNK file
   if(!f2exist && (!S_ISREG(buf2.st_mode) && !S_ISLNK(buf2.st_mode)))
   {
         if(S_ISDIR(buf2.st_mode))
         {
            return cpf2d(f1,f2);
         }
         printf("%s file type not recognized\n", f2);
         return 0;
   }
   if (f2exist || (!f2exist && S_ISREG(buf2.st_mode)) || (!f2exist && S_ISLNK(buf2.st_mode)))
      return cpf2f(f1, f2);
      // f2 exist and is DIR
   else 
      return cpf2d(f1,f2);
}

// cp file to file
int cpf2f(char *f1, char *f2)
{
      printf("cpf2f\n");
//   1. reject if f1 and f2 are the SAME file
//   2. if f1 is LNK and f2 exists: reject
//   3. if f1 is LNK and f2 does not exist: create LNK file f2 SAME as f1
//   4:
//      open f1 for READ;
//      open f2 for O_WRONLY|O_CREAT|O_TRUNC, mode=mode_of_f1;
//      copy f1 to f2
}

int cpf2d(char *f1, char *f2)
{
      printf("cpf2d\n");
//   1. search DIR f2 for basename(f1)
//      (use opendir(), readdir())
     // x=basename(f1); 
     // if x not in f2/ ==>        cpf2f(f1, f2/x)
     // if x already in f2/: 
     //      if f2/x is a file ==> cpf2f(f1, f2/x)
     //      if f2/x is a DIR  ==> cpf2d(f1, f2/x)

}



int cpd2d(char *f1, char *f2)
{
      printf("cpd2d\n");
    // recursively cp dir into dir    
}