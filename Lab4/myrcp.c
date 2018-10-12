#include "header.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: rcp SRC DST\n");
    return 0;
  }
  return myrcp(argv[1], argv[2]);
}
