#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char byte='f';
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  // fprintf(1,"%d,%d\n",p1[0],p1[1]);
  // fprintf(1,"%d,%d\n",p2[0],p2[1]);
  if(fork()==0){
    close(p1[1]);
    close(p2[0]);
    char receive_char;
    read(p1[0],&receive_char,1);
    if(receive_char==byte){
      fprintf(1,"%d: received ping\n",getpid());
      close(p1[0]);
      write(p2[1],&byte,1);
      close(p2[1]);
    }else{
      fprintf(1,"error:child receive %c\n",receive_char);
      exit(1);
    }
  }else{
      close(p1[0]);
      close(p2[1]);
      write(p1[1],&byte,1);
      char receive_char;
      read(p2[0],&receive_char,1);
      if(receive_char==byte){
        fprintf(1,"%d: received pong\n",getpid());
      }else{
        fprintf(1,"error:parent receive %c\n",receive_char);
        exit(1);
      }
  }
  exit(0);
}
