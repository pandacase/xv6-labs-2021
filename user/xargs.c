#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define ARGSIZE 64
#define BUFSIZE 1024

void
runcmd(char *cmd, char *argv[])
{
  if (fork() == 0) {
    exec(cmd, argv);
  } else {
    wait(0);
  }
  return;
}

int
main(int argc, char *argv[])
{
  char *argv_buf[ARGSIZE];  //!< argv buffer: argv + stdin
  char buf[BUFSIZE];        //!< stdin buffer
  int argv_idx = 0;         //!< argv buffer inx
  int stdin_start = 0;      //!< indicate where the stdin-idx start
  char *p_start = buf;      //!< a arg string

  // read from argv
  for (int i = 1; i < argc; ++i) {
    argv_buf[argv_idx++] = argv[i];
  }
  stdin_start = argv_idx;

  // read from stdin
  for (
    int i = 0;
    read(0, buf + i, 1) != 0 && i < BUFSIZE;
    ++i
  ) {
    char *p = buf + i;
    
    if (*p == ' ' || *p == '\n') {
      char endl_flag = *p;
      
      *p = '\0';
      argv_buf[argv_idx++] = p_start;
      
      if (i + 1 < BUFSIZE) {
        p_start = p + 1;
      }

      if (endl_flag == '\n') {
        argv_buf[argv_idx] = 0; // (NULL)
        runcmd(argv[1], argv_buf);

        argv_idx = stdin_start;
      }
    }
  }

  exit(0);
}