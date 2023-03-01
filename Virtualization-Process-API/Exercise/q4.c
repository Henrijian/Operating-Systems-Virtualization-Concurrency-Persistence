#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("parent PID = %d\n", (int)getpid());
    fflush(stdout);
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child PID = %d\n", (int)getpid());
        char* myargs[] = {"ls", ".", NULL};
        execvp(myargs[0], myargs);
        // execvp(myargs[0], myargs);
    } else {
        // parent goes down this path (main)
    }
    return 0;
}