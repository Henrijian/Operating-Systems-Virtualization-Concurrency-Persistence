#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("parent PID = %d\n", (int)getpid());
    fflush(stdout);
    
    int p[2];
    int rc = pipe(p);
    if (rc < 0) {
        // pipe failed
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child PID = %d\n", (int)getpid());
        
        close(p[1]);
        const size_t bufSize = 512;
        char buf[bufSize];
        while (read(p[0], buf, bufSize) > 0) {
            printf("%s\n", buf);
        }
        
        printf("exit (PID: %d)\n", (int)getpid());
    } else {
        // parent goes down this path (main)
        char* str = strdup("Hello, world");
        int strLen = strlen(str);
        write(p[1], str, strLen);
        close(p[1]);

        int child = wait(&rc);
        printf("child (%d) terminated\n", child);

        printf("exit (PID: %d)\n", (int)getpid());
    }
    return 0;
}