#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {

  pid_t pid = fork();

  if ( pid == 0 ) {
    printf("Child\n");
    exit(1);
  } else {
    printf("Parent\n");
    int p;
    pid_t c = wait(&p);
    printf("c=%lld p=%d\n", (long long)c, p);
  }

  exit(0);

}
