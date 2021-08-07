#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_arr(int p[2]){
    printf("0:%d  1:%d\n",p[0],p[1]);
}

int main(int argc, const char *argv[])
{
    char buf[10];
    printf("%d",read(0,buf,10));
    printf("%s",buf);
    
    exit(0);
}


