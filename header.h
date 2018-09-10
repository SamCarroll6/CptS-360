#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	char name[64];
	char type;
	struct node *child, *parent, *sibling;
}NODE;

NODE *root, *cwd, *start;
char line[128];
char command[64], pathname[64];
char dname[64], bname[64];
char *name[100];
int n;
FILE *fp;

/*
 * Writes to file to store contents of most recent
 * tree
 */
int writetofile(char * filename)
{
	fp = fopen(filename,"w");
	if(fp)
	{
		// TODO print full tree to file for storage
		fclose(fp);
		return 1;
	}
	return 0;
}

/*
 * Adds code for reading contents from file.
 * File will contain previously saved information
 * from most recent tree that was saved
 */
int readfromfile(char *filename)
{
	char value[128];
	int i = 0;
	fp = fopen(filename, "r");
	if(fp)
	{
		while(fgets(value,128,fp) != 0)
		{
			// TODO add code for reading contents from file and 
			// adding to kernel tree
		}
		fclose(fp);
		return 1;
	}
	return 0;
}

/* 
 * Function returns number of '/' 
 * seperated tokens in a path string to give 
 * the number of directories included
 */
int tokenize(char *pathname)
{
	int ret = 0;
	int i = 0;
	char hold[128];
	// Copy pathname to avoid modifying original string
	strcpy(hold, pathname);	
	char *token = strtok(hold, "/");
	while(token)
	{
		if(token != NULL)
		{
			name[ret] = token;
		}
		ret++;
		token = strtok(NULL, "/");
	}
	for(i = 0; i < ret-1; i++)
	{
		strcat(dname, name[i]);
	}
	strcpy(bname, name[ret - 1]);
	// return number of directories in pathname
	return ret;
}

NODE *search_child(NODE *parent, char *name)
{
	NODE *childlist = parent->child;	
	while(childlist)
	{
		if(strcmp(childlist->name, name) == 0)
		{
			return childlist;
		}
		childlist = childlist->sibling;
	}
	return 0;
}

NODE *path2node(char *pathname)
{
	int numpath = 0;
	NODE *p;
	if(pathname[0] = '/')
	{
		start = root;
	}
	else
	{
		start = cwd;
	}
	p = start;
	numpath = tokenize(pathname);
	for(int i = 0; i < numpath; i++)
	{
		p = search_child(p, name[i]);
		if(p == 0)
		{
			return 0;
		}	
	}
	return p;
}

int mkdir(char *pathname)
{
	
}










