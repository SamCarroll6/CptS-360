#include "header.h"

/*
 * Main function to check my myprintf function, must be compiled in 32 bit
 * architecture to work. "gcc -m32 myprints.c" will work in Linux. 
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
   for(i = 0; env[i] != NULL; i++)
   {
      myprintf("env #%d: %s\n", i, env[i]);
   } 
}

