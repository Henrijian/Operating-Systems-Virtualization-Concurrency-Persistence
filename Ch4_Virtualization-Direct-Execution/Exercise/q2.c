#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

// get the elapsed time since 1970 to the current in microseconds 
long getCurrentTime() {
    struct timeval timeStamp;
    int returnCode = gettimeofday(&timeStamp, NULL);
    if (returnCode == -1) {
        int errorCode = errno;
        printf("getCurrentTime() failed, error code = %d\n", errorCode);
        return errorCode;
    }
    return timeStamp.tv_sec * 1000 + timeStamp.tv_usec;
}

// Measuring the cost of a context switch is a little trickier. The lmbench benchmark does so by
// running two processes on a single CPU, and setting up two UNIX pipes between them; a pipe is just one of
// many ways processes in a UNIX system can communicate with one another. The first process then issues a
// write to the first pipe, and waits for a read on the second; upon seeing the first process waiting for
// something to read from the second pipe, the OS puts the first process in the blocked state, and switches to
// the other process, which reads from the first pipe and then writes to the second. When the second
// process tries to read from the first pipe again, it blocks, and thus the back-and-forth cycle of
// communication continues. By measuring the cost of communicating like this repeatedly, lmbench can make a good
// estimate of the cost of a context switch. You can try to re-create something similar here, using pipes, or
// perhaps some other communication mechanism such as UNIX sockets.
int main(int argc, char* argv[]) {
    int returnCode;
    
    int pipe1[2];
    returnCode = pipe(pipe1);
    if (returnCode < 0) {
        // pipe failed
        fprintf(stderr, "pipe() for pipe1 failed\n");
        exit(1);
    }

    int pipe2[2];
    returnCode = pipe(pipe2);
    if (returnCode < 0) {
        // pipe failed
        fprintf(stderr, "pipe() for pipe2 failed\n");
        exit(1);
    }

    returnCode = fork();
    if (returnCode < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    
    const int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE] = {0};
    const int ITER_TIMES = 1000;
    int i;
    struct timeval timeStamp;
    long prevTime, currTime, timeDiff, elapsedTime = 0; // in microseconds
    double avgTime; // in microseconds
    char* endPtr;
    if (returnCode == 0) {
        // child (new process)
        printf("child PID = %d\n", (int)getpid());

        close(pipe1[1]);
        for (i = 0; i < ITER_TIMES; i++) {
            if (read(pipe1[0], buffer, BUFFER_SIZE) > 0) {
                // printf("buffer = %s (PID: %d)\n", buffer, (int)getpid());
                currTime = getCurrentTime();
                // printf("currTime = %ld (PID: %d)\n", currTime, (int)getpid());
                prevTime = strtol(buffer, &endPtr, 10);
                // printf("prevTime = %ld (PID: %d)\n", prevTime, (int)getpid());
                timeDiff = currTime - prevTime;
                // printf("timeDiff = %ld (PID: %d)\n", timeDiff, (int)getpid());
                elapsedTime += timeDiff;

                sprintf(buffer, "%ld", currTime);
                write(pipe2[1], buffer, BUFFER_SIZE);
            }
        }
        close(pipe2[1]);
        avgTime = (double) elapsedTime / ITER_TIMES;
        printf("each context switch time = %f\n", avgTime);
    } else {
        // parent goes down this path (main)
        printf("parent PID = %d\n", (int)getpid());

        currTime = getCurrentTime();
        sprintf(buffer, "%ld", currTime);
        write(pipe1[1], buffer, BUFFER_SIZE);
        
        close(pipe2[1]);
        for (i = 0; i < ITER_TIMES; i++) {
            if (read(pipe2[0], buffer, BUFFER_SIZE) > 0) {
                // printf("buffer = %s (PID: %d)\n", buffer, (int)getpid());
                currTime = getCurrentTime();
                // printf("currTime = %ld (PID: %d)\n", currTime, (int)getpid());
                prevTime = strtol(buffer, &endPtr, 10);
                // printf("prevTime = %ld (PID: %d)\n", prevTime, (int)getpid());
                timeDiff = currTime - prevTime;
                // printf("timeDiff = %ld (PID: %d)\n", timeDiff, (int)getpid());
                elapsedTime += timeDiff;

                sprintf(buffer, "%ld", currTime);
                write(pipe1[1], buffer, BUFFER_SIZE);
            }
        }
        close(pipe1[1]);
        avgTime = (double) elapsedTime / ITER_TIMES;
        printf("each context switch time = %f\n", avgTime);

        int statusCode = 0;
        int child = wait(&statusCode);
        printf("child (%d) terminated\n", child);
    }
    printf("exit (PID: %d)\n", (int)getpid());
    return 0;
}