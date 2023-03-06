#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("parent PID = %d\n", (int)getpid());
    fflush(stdout);
    int strLen = 1000;
    int fd = open("q2.txt", O_RDWR | O_CREAT);
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child PID = %d\n", (int)getpid());
        char* str = strdup("child");
        strLen = strlen(str);
        write(fd, str, (size_t)strLen);
        free(str);
    } else {
        // parent goes down this path (main)
        char* str = strdup("parent");
        strLen = strlen(str);
        write(fd, str, (size_t)strLen);
        free(str);
    }
    return 0;
}