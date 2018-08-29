#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

// K.C. Wang supplied printu and rpu function:

int main(int argc, char *argv[], char *env[])
{
    printd(-55);
    printd(100);
    printx(1000);
    printo(1000);
}

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
   printu(x);
}

int printx(u32 x)
{
   BASE = 16;
   putchar('0');
   putchar('x');
   printu(x);
}

int printo(u32 x)
{
   BASE = 8;
   putchar('0');
   putchar('o');
   printu(x);
}
