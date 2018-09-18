#include "header.h"

/*
 * This function handles reading in the users input and tokenizing
 * that input into either a commandname or pathname.
 */ 
int readinput(void)
{
	char *token;
	// Reset command and pathname global variables
	memset(&command[0], NULL, sizeof(command));
	memset(&pathname[0], NULL, sizeof(pathname));
	printf("Command: ");
	// read in line
	fgets(line, sizeof(line), stdin);
	// Remove \n read in with fgets
	strtok(line, "\n");
	token = strtok(line, " ");
	// Find value which will represent pathname
	token = strtok(NULL, " ");
	// if token or line is null don't strcpy it
	if(token)
	{
		strcpy(pathname, token);
	}
	if(line)
	{
		strcpy(command, line);
	}
}

// Array of function pointers with parameters
int (*Farrpars[8])(char *pathname) = {mkdir, creat, rm, rmdir, cd, save, reload, lspar};
// Array of function pointers with no parameters
void (*Farrnon[6])(void) = {help, ls, clear, pwd, help, cdvoid};

int main(int argc, char* argv[])
{
	// Initialize root node and set cwd to root
	initroot();
	printf("Enter '? or 'help' for menu\n");
	cwd = root;
	int val = -1;
	// Runs program, exits if quit command is entered.
	while(1)
	{
		readinput();
		// if pathname isnt NULL check the array that takes 
		// parameters
		if(pathname[0] != NULL)
		{
			val = findspot1(command);
			if(val >= 0 && val < 8)
			{
				(*Farrpars[val])(pathname);
			}
		}
		// If command is quit run quit function and exit the program
		else if(strcmp(command, "quit") == 0)
		{
			Quit(root);
			printf("\n");
			return 0;
		}	
		// If pathname is NULL search array with no parameter functions
		// for given command
		else if(pathname[0] == NULL)
		{
			val = findspot2(command);
			if(val >= 0 && val < 6)
			{
				(*Farrnon[val])();
			}
		}
		
	}
}

// Early test function to see how functions perform. 
void tester(void)
{
	printf("Mkdir/creat files test:\n");
	mkdir("/dir1");
	mkdir("/dir2");
	mkdir("dir3");
	mkdir("/dir2/dir4");
	mkdir("/dir9/dir5");
	mkdir("/");
	creat("/f1");
	creat("f2");
	mkdir("f2");
	creat("/dir3/f3");
	printf("\n\nCD try:\n");
	cd("dir2");
	cd("/dir3");
	cd("/");
	cd("dir3/f3");
	ls();
	cd("/dir3/");
	mkdir("dir1");
	cd("dir1");
	creat("f1");
	mkdir("/f2");
	ls();
	pwd();
	cd("/");
	pwd();
	printf("\n\nTry rmdir:\n");
	//rmdir("dir1");
	rmdir("dir2");
	//cd("dir1");
	rmdir("/");
	ls();
	rm("f1");
	cd("dir1");
	rm("f1");
	ls();
	cd("/");
	ls();
	// mkdir("d1");
	// creat("f1");
	// rmdir("d1");
	// rm("f1");
	// ls();
}

