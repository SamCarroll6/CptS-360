#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

// K.C. Wang supplied printu and rpu function:

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

/**********************************************/

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

int printc(char x)
{
   putchar(x);
}

int prints(char* x)
{
  while(*x)
  { 
	putchar(*x);
	x++;
  }
}

int myprintf(char *fmt, ...)
{
  char *cp = fmt;
  int *ip = (int *)(&fmt + 1);
  while(*cp)
  {
	if(*cp != '%')
	{
	   putchar(*cp);
	}
	else
	{
	   cp++;
	   switch((char)*cp)
	 {
	   case 'c':
	      printc((char) *ip++);
	      break; 
	   case 'd':
	      printd((int) *ip++);
	      break;
	   case 'x':
	      printx((u32) *ip++);
	      break;
	   case 'o':
	      printo((u32) *ip++);
	      break;
	   case 's':
	      prints((char *) *ip++);
	      break;
	   case 'u':
	      printu((u32) *ip++);
	      break; 
	 }
	}
	cp++;   
  }   
}
#endif
