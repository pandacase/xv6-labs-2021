#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd = open(argv[1], O_CREATE);
  close(fd);
  int tmp = write_a(argv[1], "hihi\n", 6);
  if (tmp == 6) {
    printf("yes\n");
  } else {
    printf("no, %d\n", tmp);
  }
  exit(0);
}
