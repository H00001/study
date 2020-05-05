#include<string.h>
       #include <sys/types.h>
       #include <sys/wait.h>
int mysystem(char* cmdstring, char* buf, int len)
{
  int   fd[2];
  pid_t pid;
  int   n, count;
  memset(buf, 0, len);
  if (pipe(fd) < 0)
    return -1;
  if ((pid = fork()) < 0)
    return -1;
  else if (pid > 0)     /* parent process */
  {
    close(fd[1]);     /* close write end */
    count = 0;
    while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
      count += n;
    close(fd[0]);
    if (waitpid(pid, NULL, 0) > 0)
      return -1;
  }
  else                  /* child process */
  {
    close(fd[0]);     /* close read end */
    if (fd[1] != STDOUT_FILENO)
    {
      if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
      {
        return -1;
      }
      close(fd[1]);
    }
    if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1)
    return -1;
  }
  return 0;
}
int main(){
        char buf[2048];
        memset(buf,0,200);
        mysystem("route",buf,2048);
        printf("%s",buf);
        return 0;
}
