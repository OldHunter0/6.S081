#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int pre[2])
{
  // printf("process %d enter sieve,with pre[0]=%d,pre[1]=%d\n",getpid(),pre[0],pre[1]);
  int prime;
  if (read(pre[0], &prime, 4) > 0)
  {
    printf("prime %d\n", prime);
    int num;
    if (read(pre[0], &num, 4) != 0)
    {
      int next[2];
      pipe(next);
      // printf("process %d can read from pipe,build next[0]=%d,next[1]=%d\n",getpid(),next[0],next[1]);
      if (fork() != 0)
      {
        close(next[0]);
        do
        {
          if (num % prime != 0)
          {
            write(next[1], &num, 4);
          }
        } while (read(pre[0], &num, 4) > 0);
        close(pre[0]);
        close(next[1]);
        int status;
        wait(&status);
      }
      else
      {
        close(next[1]);
        // printf("child process %d\n",getpid());
        sieve(next);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  int pre[2];
  pipe(pre);
  for (int i = 2; i <= 35; i++)
  {
    write(pre[1], &i, 4);
  }
  close(pre[1]);
  sieve(pre);
  exit(0);
}
