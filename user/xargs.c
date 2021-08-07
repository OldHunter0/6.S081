#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_str_arr(char *argv[]){
  for(int i=0;argv[i]!=0;i++){
    printf("argv[%d]: %s\n",i,argv[i]);
  }
}

int main(int argc, char *argv[])
{
  char *input[64];
  int i;
  int j;
  char buf[512];
  char *begin = buf;
  // printf("argc: %d\n",argc);
  // print_str_arr(argv);
  if (argc <= 1)
  {
    printf( "usage: xargs command \n");
    exit(1);
  }

  for (i = 1; i < argc; i++)
  {
    input[i-1]=argv[i];
  }
  // print_str_arr(input);
  int n=read(0, buf, 512);
  // printf("n: %d\n",n);
  // printf("%s",buf);
  j = i-1;
  if ( n>= 512)
  {
    printf("input is too long!\n");
    exit(1);
  }
  if(n<=0){
    printf("input error\n");
    exit(1);
  }
  // printf("buf:%s\n",buf );
  for (int k = 0; k < n; k++)
  {
    if (buf[k] == ' ')
    {
      buf[k] = 0;
      input[j++] = begin;
      begin = buf + k + 1;
    }
    else if (buf[k] == '\n')
    {
      buf[k] = 0;
      input[j] = begin;
      input[j + 1] = 0;
      begin=buf+k+1;
      if (fork() == 0)
      {
        // print_str_arr(input);
        exec(input[0], input);
      }
      else
      {
        int stat;
        wait(&stat);
        j=i-1;
        input[j]=0;
      }
    }
  }
  exit(0);
}
