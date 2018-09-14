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
	printf("Root initialized OK\n");
}

/*
 * Writes to file to store contents of most recent
 * tree
 */
int save(char * filename)
{
	fp = fopen(filename,"w");
	if(fp)
	{
		traverse(root, fp);
		fclose(fp);
		printf("---- save OK ----\n");
		return 1;
	}
	printf("Unable to find file\n");
	return 0;
}

void traverse(NODE *parentprint, FILE *fp)
{
	NODE *passer = parentprint;
	savehelp(passer, fp, parentprint->type);
	if(parentprint->child)
	{
		fprintf(fp, "\n");
		passer = parentprint->child;
		traverse(passer, fp);
	}
	if(parentprint->sibling && parentprint->parent != parentprint)
	{
		fprintf(fp, "\n");
		passer = parentprint->sibling;
		traverse(passer, fp);
	}
}

void savehelp(NODE *parentprint, FILE *fp, char type)
{
	if(parentprint == parentprint->parent)
	{
		fprintf(fp, "%c %s", type, parentprint->name);
		return;
	}
	savehelp(parentprint->parent, fp, type);
	if(parentprint->parent != root)
	{
		fprintf(fp, "/%s", parentprint->name);
	}
	else
	{
		fprintf(fp, "%s", parentprint->name);
	}
}

/*
 * Adds code for reading contents from file.
 * File will contain previously saved information
 * from most recent tree that was saved
 */
int reload(char *filename)
{
	char value[128];
	char *token;
	int i = 0;
	fp = fopen(filename, "r");
	if(fp)
	{
		while(fgets(value,128,fp) != 0)
		{
			strtok(value, "\n");
			token = strtok(value, " ");
			token = strtok(NULL, " ");
			if(strcmp(value, "D") == 0 || strcmp(value, "d") == 0)
			{
				printf("token = %s\n", token);
				mkdir(token);
			}
			else if(strcmp(value, "F") == 0 || strcmp(value, "f") == 0)
			{
				printf("creat token = %s\n", token);
				creat(token);
			}
			// TODO add code for reading contents from file and 
			// adding to kernel tree
		}
		fclose(fp);
		printf("---- reload OK ----\n");
		return 1;
	}
	printf("Unable to find file\n");
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
int devbname(char *pathname)
{
	int i = 1;
	if(pathname[0] == '/')
	{
		dname[0] = '/';
	}
	if(name[1] != NULL && dname[0] == '/')
	{
		strcat(dname, name[0]);
	}
	else if(name[1] != NULL)
	{
		strcpy(dname, name[0]);
	}
	for(i; i < n-1; i++)
	{
		strcat(dname, "/");
		strcat(dname, name[i]);
	}
	strcpy(bname, name[n - 1]);       
}

/*
 * Search child node of parent for a match to provided name
 * helper for various command functions including mkdir and
 * creat.
*/
NODE *search_child(NODE *parent, char *name)
{
	NODE *childlist = parent->child;	
	// While childlist isn't Null/end of list
	while(childlist)
	{
		// if current childlist node name equals the given name
		// return the node if it matches, otherwise return 0 if 
		// it doesn't exist.
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
	// Set start node to either root or cwd
	// based on pathname
	if(pathname[0] == '/')
	{
		start = root;
	}
	else
	{
		start = cwd;
	}
	// tokenize the path name to break it up into seperate names
	numpath = tokenize(pathname);
	// Set new variable *p to start so start value doesnt get changed.
	NODE *p = start;
	for(int i = 0; i < numpath; i++)
	{
		// Search child for next stored name, if NULL return 0
		// as directory does not currently exist
		p = search_child(p, name[i]);
		if(p == 0)
		{
			return 0;
		}
		// Return 0 and print error if file type in pathname 
		// is not a directory type.
		if(p->type != 'd' && i+1 != numpath)
		{
			// Check if matched name from search child is directory. 
			// If it is a file it will return 1 so calling function 
			// assumes that a matched name was found and won't make 
			// a new one.
			printf("Error: %s: Not a Directory\n", p->name);
			return 1;
		}	
	}
	// If appropriate path found return the last node
	return p;
}


int mkdir(char *pathname)
{
	printf("**** mkdir %s\n", pathname);
	// Needs to reset first two values of name
	// as well as bname and dname for reuse.
	name[0] = NULL;
	name[1] = NULL;
	memset(&bname[0], NULL, sizeof(bname));
	memset(&dname[0], NULL, sizeof(dname));
	NODE *errcheck = path2node(pathname);
	// Check for existing node with same name
	if(errcheck != NULL && errcheck != 1)
	{
		// If the node exists return 0 and print error
		printf("Error: %s: Directory already exists\n", pathname);
		return 0;
	}
	else if(errcheck == 1)
	{
		return 0;
	}
	devbname(pathname);
	NODE *newparent = path2node(dname);
	//printf("%s\n", newparent->name);
	if(newparent == NULL)
	{
		printf("dname = %s\n", dname);
		printf("bname = %s\n", bname);
		printf("Error: %s: Path not found\n", pathname);
		return 0;
	}
	if(bname == "/")
	{
		printf("Error: Can't name new node '/'\n");
		return 0;
	}
	if(newparent->child != NULL)
	{
		NODE *newchild = newparent->child;
		while(newchild->sibling)
		{
			newchild = newchild->sibling;
		}
		newchild->sibling = newnode(bname, 'd', newparent);
		printf("---- mkdir OK ----\n");
		return 1;
	}
	else
	{
		newparent->child = newnode(bname, 'd', newparent);
		printf("---- mkdir OK ----\n");
		return 1;
	}
}

int creat(char *pathname)
{
	printf("**** creat %s\n", pathname);
	// Needs to reset first two values of name
	// as well as bname and dname for reuse.
	name[0] = NULL;
	name[1] = NULL;
	memset(&bname[0], NULL, sizeof(bname));
	memset(&dname[0], NULL, sizeof(dname));
	NODE *errcheck = path2node(pathname);
	// Check for existing node with same name
	if(errcheck != NULL && errcheck != 1)
	{
		// If the node exists return 0 and print error
		printf("Error: %s: Directory already exists\n", pathname);
		return 0;
	}
	else if(errcheck == 1)
	{
		return 0;
	}
	devbname(pathname);
	NODE *newparent = path2node(dname);
	//printf("%s\n", newparent->name);
	if(newparent == NULL)
	{
		printf("Error: %s: Path not found\n", pathname);
		return 0;
	}
	if(bname == "/")
	{
		printf("Error: Can't name new node '/'\n");
		return 0;
	}
	if(newparent->child != NULL)
	{
		NODE *newchild = newparent->child;
		while(newchild->sibling)
		{
			newchild = newchild->sibling;
		}
		newchild->sibling = newnode(bname, 'f', newparent);
		printf("---- creat OK ----\n");
		return 1;
	}
	else
	{
		newparent->child = newnode(bname, 'f', newparent);
		printf("---- creat OK ----\n");
		return 1;
	}
}

int cd(char *pathname)
{
	printf("**** cd %s\n", pathname);
	name[0] = NULL;
	name[1] = NULL;
	memset(&bname[0], NULL, sizeof(bname));
	memset(&dname[0], NULL, sizeof(dname));

	NODE *newcwd = path2node(pathname);
	if(newcwd == 1)
	{
		return 0;
	}
	else if(newcwd && newcwd->type == 'd')
	{
		cwd = newcwd;
		printf("---- cd OK ----\n");
		return 1;
	}
	else if(newcwd == NULL)
	{
		printf("Error: Path does not exist\n");
		return 0;
	}
	printf("Error: %s: Path contains non directory type\n", pathname);
	return 0;
}

void ls(void)
{
	printf("**** ls ****\n");
	if(cwd->child)
	{
		NODE *childptr = cwd->child;
		while(childptr)
		{
			printf("%c\t%s\n", childptr->type, childptr->name);
			childptr = childptr->sibling;
		}
	}
}

void pwd(void)
{
	printf("**** pwd ****\n");
	if(cwd != root)
	{
		pwdhelper(cwd->parent);
		printf("%s\n", cwd->name);
	}
	else
	{
		printf("/\n");
	}
}

void pwdhelper(NODE *parentprint)
{
	if(parentprint == parentprint->parent)
	{
		printf("/");
		return;
	}
	pwdhelper(parentprint->parent);
	printf("%s/", parentprint->name);
}

int rmdir(char *pathname)
{
	printf("**** rmdir %s\n", pathname);
	// Node value for the node to be removed
	NODE *pathnode = path2node(pathname);
	// if pathname doesn't exist or somewhere in the pathname
	// a file is referenced as a directory
	if(pathnode == cwd)
	{
		printf("Error: Can't remove Current Working Directory\n");
		return 0;
	}
	if(pathnode == 0 || pathnode == 1)
	{
		return 0;
	}
	// Parent node of node to be removed
	NODE *checkval = pathnode->parent;
	// if the returned node is the root do not remove
	if(checkval == pathnode)
	{
		printf("Error: Can't remove root\n");
		return 0;
	}
	// If node2path returns a file instead of directory
	else if(pathnode->type != 'd')
	{
		printf("rmdir: failed to remove '%s': not a directory\n", pathnode->name);
		return 0;
	}
	// Can't remove non empty directories, this checks for that
	else if(pathnode->child)
	{
		printf("rmdir: failed to remove '%s': directory not empty\n", pathnode->name);
		return 0;
	}
	// If node to be removed is immediate child of parent 
	// account for this when removing the node.
	if(checkval->child == pathnode)
	{
		checkval->child = pathnode->sibling;
		free(pathnode);
		printf("---- rmdir OK ----\n");
		return 1;
	}
	// Find node before pathnode in sibling linked list
	checkval = checkval->child;
	while(strcmp(checkval->sibling->name, pathnode->name) != 0 && checkval)
	{
		checkval = checkval->sibling;
	}
	// Remove pathnode and set checkval sibling node to pathnodes sibling
	checkval->sibling = pathnode->sibling;
	free(pathnode);
	printf("---- rmdir OK ----\n");
	return 1;
}

int rm(char *pathname)
{
	printf("**** rm %s\n", pathname);
	// Node value for the node to be removed
	NODE *pathnode = path2node(pathname);
	// if pathname doesn't exist or somewhere in the pathname
	// a file is referenced as a directory
	if(pathnode == 0 || pathnode == 1)
	{
		return 0;
	}
	// Parent node of node to be removed
	NODE *checkval = pathnode->parent;
	// If node2path returns a directory
	if(pathnode->type == 'd')
	{
		printf("rm: failed to remove '%s': Is a directory\n", pathnode->name);
		return 0;
	}
	// If node to be removed is immediate child of parent 
	// account for this when removing the node.
	else if(checkval->child == pathnode)
	{
		checkval->child = pathnode->sibling;
		free(pathnode);
		printf("---- rm OK ----\n");
		return 1;
	}
	// Find node before pathnode in sibling linked list
	checkval = checkval->child;
	while(strcmp(checkval->sibling->name, pathnode->name) != 0 && checkval)
	{
		checkval = checkval->sibling;
	}
	// Remove pathnode and set checkval sibling node to pathnodes sibling
	checkval->sibling = pathnode->sibling;
	free(pathnode);
	printf("---- rm OK ----\n");
	return 1;
}

void clear(void)
{
	system("clear");
}

void help(void)
{
	printf("=========================== MENU ===========================\n");
	printf("mkdir rmdir ls  cd  pwd  creat  rm  save reload  quit  clear\n");
	printf("============================================================\n");
}

char *pars[7] = {"mkdir", "creat", "rm", "rmdir", "cd", "save", "reload"};
char *nopars[5] = {"help", "ls", "clear", "pwd", "?"};

int findspot1(char *commandname)
{
	int i = 0;
	for(i; i < 7; i++)
	{
		if(strcmp(commandname, pars[i]) == 0)
		{
			return i;
		}
	}
	printf("Unrecognized command\n");
	return -1;
}

int findspot2(char *commandname)
{
	int i = 0;
	for(i; i < 5; i++)
	{
		if(strcmp(commandname, nopars[i]) == 0)
		{
			return i;
		}
	}
	for(i; i < 7; i++)
	{
		if(strcmp(commandname, pars[i]) == 0)
		{
			printf("%s Missing Operand\n");
			return -1;
		}
	}
	printf("Unrecognized command\n");
	return -1;
}


void Quit(NODE *parentprint)
{
	NODE *passer = parentprint;
	Quithelp(passer, parentprint->type);
	if(parentprint->child)
	{
		printf("\n");
		passer = parentprint->child;
		Quit(passer);
	}
	if(parentprint->sibling && parentprint->parent != parentprint)
	{
		printf("\n");
		passer = parentprint->sibling;
		Quit(passer);
	}
}

void Quithelp(NODE *parentprint, char type)
{
	if(parentprint == parentprint->parent)
	{
		printf("%c %s", type, parentprint->name);
		return;
	}
	Quithelp(parentprint->parent, type);
	if(parentprint->parent != root)
	{
		printf("/%s", parentprint->name);
	}
	else
	{
		printf("%s", parentprint->name);
	}
}