#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid;
  int pipe_p2c[2], pipe_c2p[2];
  char buf[128];  // read buffer
  int n;          // n bytes readed
  
  // create 2 direction pipe
  pipe(pipe_p2c); // parent to child
  pipe(pipe_c2p); // child to parent

  // // For testing:
  // // fd 0/1/2 is std in/out/err
  // // so ... p2c[0]: 3, p2c[1]: 4, c2p[0]:5, c2p[1]:6
  // for (int i = 0; i < 2; ++i) {
  //     printf(
  //         "p2c[%d]: %d, c2p[%d]:%d \n",
  //         i, pipe_p2c[i], i, pipe_c2p[i]
  //     );
  // }

  pid = fork();
  if (pid < 0) {
    fprintf(2, "pingpong: fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    write(pipe_p2c[1], "ping!", 6);
    n = read(pipe_c2p[0], buf, sizeof buf);
    if (n > 0) {
      printf("%d: received pong\n", getpid());
    } else {
      fprintf(2, "Some error happend.\n");
      exit(1);
    }
  } else {
    n = read(pipe_p2c[0], buf, sizeof buf);
    if (n > 0) {
      printf("%d: received ping\n", getpid());
      write(pipe_c2p[1], "pong!", 6);
    } else {
      fprintf(2, "Some error happend.\n");
      exit(1);
    }
    wait(0);
  }
  exit(0);
}