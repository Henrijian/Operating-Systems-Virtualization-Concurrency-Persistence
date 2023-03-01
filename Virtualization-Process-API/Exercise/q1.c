#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("parent PID = %d\n", (int)getpid());
    fflush(stdout);
    int x = 100;
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child PID = %d\n", (int)getpid());
        x = 50;
        printf("x = %d (pid:%d)\n", x, (int)getpid());
    } else {
        // parent goes down this path (main)
        x = 150;
        printf("x = %d (pid:%d)\n", x, (int)getpid());
    }
    return 0;
}