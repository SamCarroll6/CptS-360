#include "header.h"

int main(int argc, char* argv[])
{
	initroot();
	cwd = root;
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
}
