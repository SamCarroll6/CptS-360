#include "header.h"

int main(int argc, char* argv[])
{
	initroot();
	cwd = root;
	char *token;
	while(1)
	{
		printf("Command: ");
		fgets(line, sizeof(line), stdin);
		strtok(line, "\n");
		token = strtok(line, " ");
		if(strcmp(token, line) != 0)
		{
			token = strtok(NULL, " ");
		}
		strcpy(pathname, token);
		strcpy(command, line);
		printf("Command: %s\n", command);
		printf("pathname: %s\n", pathname);
		//printf("Command: %s\nPathname: %s\n", command, pathname);
		if(strcmp(line, "clear") == 0)
		{
			clear();
		}
		if(strcmp(line, "quit") == 0)
		{
			return 0;
		}
	}
}

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
