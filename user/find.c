#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', 1);
  return buf;
}

void find(char *path, char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  // printf("search %s in %s\n",file_name, path);
  if ((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  if (st.type != T_DIR){
    fprintf(2, "%s: not a valid path\n", path);
    close(fd);
    return;
  }
  else{
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      char *cur_name= fmtname(buf);

      // printf("scan %s<detect space>\n",cur_name);
      // printf("compare to '.': %d\n",strcmp(cur_name,"."));
    
      if(st.type==T_DIR && (strcmp(cur_name,".")!=0)&&(strcmp(cur_name,"..")!=0)){
        // printf("find in %s\n",cur_name);
        find(buf,file_name);
      }else if(st.type==T_FILE&& strcmp(cur_name,file_name)==0){
        printf("%s\n",buf);
      }
    }
    close(fd);
  }
  return;
}

int main(int argc, char *argv[])
{
  if (argc == 2){
    find(".", argv[1]);
    exit(0);
  }
  else if (argc == 3){
    find(argv[1], argv[2]);
  }
  else{
    printf("Usage: find path files...\n");
  }
  exit(0);
}