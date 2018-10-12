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
   if(!S_ISREG(buf1.st_mode)) 
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
                  mkdir(f2, buf1.st_mode);
            }
            else
            {
                  char *hold = (char*)malloc(sizeof(char) * (strlen(f2) + strlen(f1)));
                  strcpy(hold, f2);
                  char *bname = (char*)malloc(sizeof(char) * (strlen(f1)));
                  bname = basename(f1);
                  if(hold[strlen(hold) - 1] != '/')
                        strcat(hold, "/");
                  strcat(hold,bname);
                  mkdir(hold, buf1.st_mode);
                  return cpd2d(f1, hold);
            }
	      return cpd2d(f1, f2);
         }
         printf("%s file type not recognized\n", f1);
         return 0;
   }  
   //reject if f2 exists and is not REG or LNK file
   if(!f2exist && !S_ISREG(buf2.st_mode))
   {
         if(S_ISDIR(buf2.st_mode))
         {
            return cpf2d(f1,f2);
         }
         printf("%s file type not recognized\n", f2);
         return 0;
   }
   if (f2exist || (!f2exist && S_ISREG(buf2.st_mode)))
      return cpf2f(f1, f2);
      // f2 exist and is DIR
   else 
      return cpf2d(f1,f2);
}

// cp file to file
int cpf2f(char *f1, char *f2)
{
      struct stat buf1;
      struct stat buf2;
      int fp, fp2, size, r1, w1;
      lstat(f1, &buf1);
      int f2exist = lstat(f2, &buf2);
//   1. reject if f1 and f2 are the SAME file
      if(buf1.st_ino == buf2.st_ino)
      {
            printf("cp: %s and %s are the same file\n", f1, f2);
            return 0;
      }
      if(S_ISLNK(buf2.st_mode) && S_ISREG(buf1.st_mode))
      {
            fp = open(realpath(f2, NULL), O_WRONLY | O_TRUNC);
            fp2 = open(f1, O_RDONLY);
            size = buf1.st_size;
            char *buffer = (char*)malloc(size * sizeof(char));
            r1 = read(fp2, buffer, size);
            w1 = write(fp, buffer, size * sizeof(char));
            free(buffer);
            close(fp);
            close(fp2);
            return 1;
      }
//   2. if f1 is LNK and f2 exists copy contents of link to reg. 
      if(S_ISREG(buf2.st_mode) && S_ISLNK(buf1.st_mode))
      {     
            struct stat symbuf;
            stat(realpath(f1, NULL), &symbuf);
            fp = open(f1, O_RDONLY);
            fp2 = open(f2, O_WRONLY | O_TRUNC);
            size = symbuf.st_size;
            char *buffer = (char*)malloc(size * sizeof(char));
            r1 = read(fp, buffer, size);
            w1 = write(fp2, buffer, size * sizeof(char));
            free(buffer);
            close(fp);
            close(fp2);
            return 1;
      }
//    if f1 is LNK and f2 does not exist: create LNK file f2 SAME as f1.
      if(S_ISLNK(buf1.st_mode) && f2exist)
      {
            symlink(basename(realpath(f1, NULL)), f2);
            return 1;
      }
//    If both files regular files, or just file 1 is regular and file 2 doesnt exist.       
      if((S_ISREG(buf2.st_mode) && S_ISREG(buf1.st_mode)) || (S_ISREG(buf1.st_mode) && f2exist))
      {
            fp = open(f1, O_RDONLY);
            fp2 = open(f2, O_WRONLY | O_CREAT | O_TRUNC, buf1.st_mode);
            size = buf1.st_size;
            char *buffer = (char*)malloc(size * sizeof(char));
            r1 = read(fp, buffer, size);
            w1 = write(fp2, buffer, size * sizeof(char));
            free(buffer);
            close(fp);
            close(fp2);
            return 1;
      }
      // If none of the above cases occured then something is wrong with input and we return 0.
      return 0;
}

int cpf2d(char *f1, char *f2)
{
      DIR *dir = opendir(f2);
      char *bname = (char*)malloc(sizeof(char) * strlen(f1));
      strcpy(bname, f1);
      bname = basename(bname);
      struct dirent *sd;
      if(dir == NULL)
      {
            printf("Error: Directory %s not found\n", f2);
            return 0;
      }
      while((sd = readdir(dir)) != NULL)
      {
            if(!strcmp(f1, sd->d_name))
            {
                  int len = strlen(f2);

                  if(f2[len - 1] == '/')
                        strcat(f2,bname);
                  else
                  {
                        strcat(f2, "/");
                        strcat(f2, bname);
                  }
                  struct stat check;
                  stat(f2, &check);
                  if(S_ISDIR(check.st_mode))
                  {
                        close(dir);
                        return cpf2f(f1, f2); 
                  }
                  else
                  {
                        close(dir);
                        return cpf2f(f1, f2); 
                  }
            }
      }
      int len = strlen(f2);
      if(f2[len - 1] == '/')
            strcat(f2,bname);
      else
      {
            strcat(f2, "/");
            strcat(f2, bname);
      }
      close(dir);
      return cpf2f(f1, f2);
}



int cpd2d(char *f1, char *f2)
{
      struct stat buf1;
      struct stat buf2;
      struct stat buf3;
      struct dirent *sd;
      stat(f1, &buf1);
      int f2exist = stat(f2, &buf2);
      if(buf1.st_ino == buf2.st_ino)
      {
            printf("cp: %s and %s are the same directory\n", f1, f2);
            return 0;
      }
      if(f2exist)
      {
            mkdir(f2, buf1.st_mode);
      }
      char *dname = (char*)malloc(sizeof(char) * strlen(f2));
      char *hold = (char*)malloc(sizeof(char) * strlen(f2));
      char *hold2 = (char*)malloc(sizeof(char) * strlen(f2));
      strcpy(dname, f2);
      strcpy(hold, f2);
      dirname(dname);
      while(strcmp(dname, hold))
      {
            stat(dname, &buf3);
            if(buf1.st_ino == buf3.st_ino)
            {
                  printf("Can't copy into SubDir\n");
                  free(dname);
                  free(hold);
                  return 0;
            }
            strcpy(hold, dname);
            dirname(dname);
      }
      free(dname);
      free(hold);
      hold = (char*)malloc(sizeof(char) * (strlen(f1) + strlen(f2)));
      DIR *dir = opendir(f1);
      while((sd = readdir(dir)) != NULL)
      {
             if(strcmp(".", sd->d_name) && strcmp("..", sd->d_name))
             {
                  // Has to be copied, if I use f2 it gets modified in function calls.
                  strcpy(hold2, f2);
                  strcpy(hold, f1);
                  if(hold[strlen(f1) - 1] != '/')
                        strcat(hold, "/");
                  strcat(hold, sd->d_name);
                  struct stat check;
                  stat(hold, &check);
                  if(S_ISDIR(check.st_mode))
                  {
                        if(hold[strlen(f2) - 1] != '/')
                              strcat(hold2, "/");
                        strcat(hold2, sd->d_name);
                        cpd2d(hold, hold2); 
                  }
                  else
                  {
                        cpf2d(hold, hold2); 
                  }
             }
      }
}