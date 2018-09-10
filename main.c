#include "header.h"

int main(int argc, char* argv[])
{
	char *hold = "/one/two/three/four";
	writetofile("myfile.txt");
	readfromfile("myfile.txt");
	int val = tokenize(hold);
	printf("\n%s\n", hold);
	printf("%d\n", val);	
	hold = "newwords";
	val = tokenize(hold);
	printf("\n%s\n", hold);
	printf("%d\n", val);
}
