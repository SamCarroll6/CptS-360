#include "header.h"

int main(int argc, char* argv[])
{
	initroot();
	cwd = root;
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
	printf("First root child: %s\n", root->child->name);
	printf("Second root child: %s\n", root->child->sibling->name);
	printf("Third root child: %s\n", root->child->sibling->sibling->name);
	printf("Nested child: %s -> %s\n", root->child->sibling->name, root->child->sibling->child->name);
	NODE *print = root->child;
	while(print->sibling)
	{
		printf("%s->", print->name);
		print = print->sibling;
	}
	printf("\nFile: %s  Type: %c\n", print->name, print->type);
	printf("\n\nCD try:\n");
	cd("dir2");
	printf("CWD = %s\nCWD child = %s\n", cwd->name, cwd->child->name);
	cd("/");
	printf("CWD = %s\nCWD child = %s\n", cwd->name, cwd->child->name);

}
