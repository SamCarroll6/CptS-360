#include "header.h"
//#include <stdio.h>
//#include <stdlib.h>

/*typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

// K.C. Wang supplied printu and rpu function:
int printc(char x)
{
    putchar(x);
}
*/
int main(int argc, char *argv[], char *env[])
{
   int i;
   myprintf("My printf tests:\nchar check = %c\nstring check = %s\nunsigned check = %u\ninteger check = %d\noctal check = %o\nhex check = %x\n",'A', "hello there", 50, -50, 80, 100);
   myprintf("argc value: %d\n", argc);
   for(i = 0; i < argc; i++)
   {
      myprintf("argv #%d: %s\n", i, argv[i]);
   } 
}

/*
int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];
       rpu(x / BASE);
       putchar(c);
    }
}

int printu(u32 x)
{
   BASE = 10;
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}
*/
/**********************************************/
/*
int printd(int x)
{
   BASE = 10;
   if(x < 0)
   {
	putchar('-');
	x = x*(-1);
   }
   (x==0)? putchar('0') : rpu(x);   
}

int printx(u32 x)
{
   BASE = 16;
   putchar('0');
   putchar('x');
   (x==0)? putchar('0') : rpu(x);
}

int printo(u32 x)
{
   BASE = 8;
   putchar('0');
   putchar('o');
   (x==0)? putchar('0') : rpu(x);
}

int prints(char* x)
{
  while(*x)
  { 
	putchar(*x);
	x++;
  }
}

*/
