#include <stdio.h>
#include <stdlib.h>

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

int tokenize(char *pathname)
{
	
}

NODE *search_child(NODE *parent, char *name)
{

}

NODE *path2node(char *pathname)
{

}
