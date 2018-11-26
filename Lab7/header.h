#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>


/*************** type.h file *********************************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;   

#define BLKSIZE  1024

#define NMINODE    64
#define NOFT       64
#define NFD        10
#define NMOUNT      4
#define NPROC       2

typedef struct minode{
  INODE INODE;
  int dev, ino;
  int refCount;
  int dirty;
  int mounted;
  struct mntable *mptr;
}MINODE;

typedef struct oft{
  int  mode;
  int  refCount;
  MINODE *mptr;
  int  offset;
}OFT;

typedef struct proc{
  struct proc *next;
  int          pid;
  int          uid, gid;
  MINODE      *cwd;
  OFT         *fd[NFD];
}PROC;

MINODE minode[NMINODE];
MINODE *root;
PROC   proc[NPROC], *running;

char gpath[256];
char *name[64]; // assume at most 64 components in pathnames
int  n;

int  fd, dev;
int  nblocks, ninodes, bmap, imap, inode_start;
char line[256], cmd[32], pathname[256];

// Utility/Mount Functions 
int get_block(int fd, int blk, char buf[]);
int put_block(int fd, int blk, char buf[]);
int init(void);
int mountroot(char *diskname);
MINODE *iget(int dev, int ino);
int iput(MINODE *mip);
int getino(MINODE *mip, char *name2);
INODE *get_inode(int dev, int ino);
char* Parse(char* input);
char* readinput(void);
int tokenize(char *pathname);
int search(INODE *ip, char *name);

// PWD, CD, LS functions
void ls_dir(MINODE *mip);
void ls_file(MINODE* mip, char *name2);
MINODE* findval(MINODE *mip);
int checktype(MINODE *mip);
int chdir(void);
void pwd(MINODE *pr);
void rpwd(MINODE *pr);

#endif