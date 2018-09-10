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

NODE *newnode(char *name, char type, NODE *parent)
{
        NODE *ret = (NODE*)malloc(sizeof(NODE));
        ret->type = type;
        strcpy(ret->name, name);
        ret->parent = parent;
        ret->sibling = NULL;
	ret->child = NULL;
        return ret;
}


int initroot(void)
{
	root = newnode("/", 'd', NULL);
	cwd = root;
	root->sibling = root;
	root->parent = root;
}

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
	while(name[i])
	{
		name[i] = NULL;
		i++;
	}
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
	i = 0;
	while(dname[i] != '\0')
	{
		dname[i] = '\0';
		i++;
	}
	// return number of directories in pathname
	n = ret;
	return ret;
}

/*
 * Gives bname, used for commands that 
 * have a path not including an existing 
 * directory or file to remove that new 
 * directory/files name from pathname and 
 * store it
 */
int devbname(void)
{
	int i;
	if(name[1] != NULL)
	{
		strcpy(dname, name[0]);
	}
	for(i = 1; i < n-1; i++)
	{
		strcat(dname, name[i]);
	}
	strcpy(bname, name[n - 1]);       
}

NODE *search_child(NODE *parent, char *name)
{
	NODE *childlist = parent->child;	
	while(childlist)
	{
		printf("Searching child\n");
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
	//NODE *p;
if(pathname != NULL)
{
	if(pathname[0] == '/')
	{
		start = root;
	}
	else
	{
		start = cwd;
	}
	//NODE *p = (NODE*)malloc(sizeof(NODE));
	//NODE *p = start;
	numpath = tokenize(pathname);
}
	NODE *p = cwd;
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

/*
NODE *newnode(char *name, char type, NODE *parent)
{
	NODE *ret = (NODE*)malloc(sizeof(NODE));
	ret->type = type;
	strcpy(ret->name, name);
	ret->parent = parent;
	ret->sibling = NULL;
	return ret;
}
*/

int mkdir(char *pathname)
{
	// Check for existing node with same name
	if(path2node(pathname) != NULL)
	{
		// If the node exists return 0 and print error
		printf("Error: Directory already exists\n");
		return 0;
	}
	devbname();
	//printf("bname = %s", bname);
	//printf("DName = %s\n", dname);
	NODE *newparent = path2node(dname);
	if(newparent == NULL)
	{
		printf("Error: Path not found\n");
		return 0;
	}
	NODE *newchild = newparent->child;
	while(newchild->sibling)
	{
		newchild = newchild->sibling;
	}
	newchild->sibling = newnode(bname, 'd', newparent);
}









