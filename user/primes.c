#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
sieve(int pipe_left[2])
{
    int pid;
    int pipe_right[2];
    int prime_num, num;
    int sentry = -1;       // sentry to indicate the last int

    pipe(pipe_right);

    // read the first number : is prime
    read(pipe_left[0], &prime_num, sizeof prime_num);
    if (prime_num == -1) {
        exit(0);
    }
    printf("prime %d\n", prime_num);

    pid = fork();
    if (pid == 0) {
        // the next stage
        close(pipe_right[1]);   // no need write to left
        sieve(pipe_right);
    } else {
        // the current stage
        close(pipe_right[0]);   // no need read from right

        while (read(pipe_left[0], &num, sizeof num) > 0 && num != -1) {
            if ( num % prime_num ) {
                write(pipe_right[1], &num, sizeof num);
            }
        }
        close(pipe_left[0]);    // complete read from left
        
        write(pipe_right[1], &sentry, sizeof num);

        close(pipe_right[1]);   // complete write to right
        wait(0);
    }
    exit(0);
}

int
main(void)
{
    int pid;
    int pipe_main[2];
    int sentry = -1;        // sentry to indicate the last int

    pipe(pipe_main);

    pid = fork();
    if (pid < 0) {
        fprintf(2, "primes: fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        // the first child process
        close(pipe_main[1]);    // no need write to left
        sieve(pipe_main);
    } else {
        // the main(root) process
        close(pipe_main[0]);    // no need read from right

        for (int i = 2; i <= 35; ++i) {
            write(pipe_main[1], &i, sizeof i);
        }

        write(pipe_main[1], &sentry, sizeof sentry);

        close(pipe_main[1]);    // complete write to right
        wait(0);
    }

    exit(0);
}