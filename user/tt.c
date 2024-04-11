#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
func()
{
  exit(-1);
  return;
}


int
main(int argc, char *argv[])
{
  func();
  printf("hi\n");
  exit(0);
}
