# CptS-360
# Systems Programming course at Washington State University

# Lab1
Part 1 of the first assignment for the course. Prints variables as well as memory locations of those variables. Outlines the stack frame linked list as well as printing the stack contents from a variable p to main (limited to 128 entries). Must be compiled in 32 bit to properly work, with gcc the command is "gcc -m32 main.c ts.s".

# Lab1 Myprintf
Part 2 of the first assignment. It was assigned to implement my own version of the "printf()" function called "myprintf()". This was done using putchar to place each individual value in the given string, it can take any number of variables after the initial string which would be placed in the specified spot in the provided string just like the actual printf() function works. Variable types are distinguished using the same structure as the actual function (%d represents int, %s represents string, etc.).

# Lab2
Creates the general structure of a Linux kernel environment. Using simple nodes that identify as 'f' for file or 'd' for directory this has a standard root directory to start with the ability for new files/directories to be added with appropriate commands. It supports the commands cd, ls, mkdir, creat, clear, quit, rm, rmdir, save, reload, and pwd.
