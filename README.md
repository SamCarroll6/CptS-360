# CptS-360
# Systems Programming course at Washington State University

# Lab1
Part 1 of the first assignment for the course. Prints variables as well as memory locations of those variables. Outlines the stack frame linked list as well as printing the stack contents from a variable p to main (limited to 128 entries). Must be compiled in 32 bit to properly work, with gcc the command is "gcc -m32 main.c ts.s".

# Lab1 Myprintf
Part 2 of the first assignment. It was assigned to implement my own version of the "printf()" function called "myprintf()". This was done using putchar to place each individual value in the given string, it can take any number of variables after the initial string which would be placed in the specified spot in the provided string just like the actual printf() function works. Variable types are distinguished using the same structure as the actual function (%d represents int, %s represents string, etc.).

# Lab2
Creates the general structure of a Linux kernel environment. Using simple nodes that identify as 'f' for file or 'd' for directory. The structure has a standard root directory to start with and the ability for new files/directories to be added with appropriate commands. It supports the commands cd, ls, mkdir, creat, clear, quit, rm, rmdir, save, reload, and pwd.

# Lab3
Uses the fork() and exec() functions to create a Linux kernel environment. Can run any unix commands. Also allows for single piping, but it can't handle multiple piping. The program itself works by searching every path provided in the environment until it finds the proper command, it then executes that command with any accompanying arguments being passed in as well. It can run shell programs too. 

# Lab4
My implementation of the cp -r command from linux. Allows the user to copy file to file, file to directory, or directory to directory. In the case of symbolic links in file to file copying it will give the newly copied file the same symbolic link that the original holds. Can work with copying to existing directories and can also create new ones if they don't exist.

# PreLab6 
This must be run on an EXT2 filesystem, the there is a shell script included for mounting a disk if needed. This was a prelab to assignment 6 of this course. We were tasked with printing information in realtion to the file system. Things such as the Super block, Inode block, Inode map, Block map, and a few others. This allowed us to get comfortable with finding and accessing this information for Lab6.

# Lab6
This must run on an EXT2 filesystem, the provided example is called diskimage. For this we were tasked with printing all of the allocated blocks on a file. This involved traversing the 12 direct blocks, all indirect blocks, and all double indirect blocks. The program only prints allocated blocks so it can be tested on the given diskimage with X/tinyfile for just direct blocks, Y/bigfile for the first indirect block, and then Z/hugefile for checking double indirect blocks (first letter of names may need to be capitalized I can't recall, i.e. Z/Hugefile if lowercase doesn't work.) 

# Lab7
This will again run on an EXT2 filesystem
