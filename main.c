#include "header.h"

int main(int argc, char* argv[])
{
	initroot();
	cwd = root;
	mkdir("/dir1");
	mkdir("/dir2");
	mkdir("/dir3");
	mkdir("/dir2/dir4");
	mkdir("/dir9/dir5");
	mkdir("/");
	creat("f1");
	printf("First root child: %s\n", root->child->name);
	printf("Second root child: %s\n", root->child->sibling->name);
	printf("Third root child: %s\n", root->child->sibling->sibling->name);
	printf("Nested child: %s -> %s\n", root->child->sibling->name, root->child->sibling->child->name);

}
