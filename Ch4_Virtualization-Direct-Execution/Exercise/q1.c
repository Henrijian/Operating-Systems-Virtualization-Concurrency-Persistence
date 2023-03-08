#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

// In this exercise, you’ll measure the costs of a system call and context switch.
// Measuring the cost of a system call is relatively easy. For example, you could repeatedly call
// a simple system call (e.g., performing a 0-byte read), and time how long it takes; dividing the time
// by the number of iterations gives you an estimate of the cost of a system call.

// One thing you’ll have to take into account is the precision and accuracy of your timer. A
// typical timer that you can use is `gettimeofday()`; read the man page for details. What you’ll see there
// is that `gettimeofday()` returns the time in microseconds since 1970; however, this does not mean
// that the timer is precise to the microsecond. Measure back-to-back calls to `gettimeofday()` to learn
// something about how precise the timer really is; this will tell you how many iterations of your null
// system-call test you’ll have to run in order to get a good measurement result. If `gettimeofday()` is not
// precise enough for you, you might look into using the `rdtsc` instruction available on x86 machines. 
int main(int argc, char* argv[]) {
    const int BUFFER_SIZE = 10;
    char buffer[BUFFER_SIZE] = {};

    
    int returnCode, errorCode;
    
    struct timeval startTime;
    returnCode = gettimeofday(&startTime, NULL);
    if (returnCode == -1) {
        errorCode = errno;
        printf("gettimeofday() for the start time failed, error code = %d\n", errorCode);
        return errorCode;
    }
    printf("start time = %ld seconds, %d microseconds\n", startTime.tv_sec, startTime.tv_usec);

    const int ITER_TIMES = 1000;
    for (int i = 0; i < ITER_TIMES; i++) {
        size_t readSize = read(stdin, buffer, 0);
    }

    struct timeval endTime;
    returnCode = gettimeofday(&endTime, NULL);
    if (returnCode == -1) {
        errorCode = errno;
        printf("gettimeofday() for the end time failed, error code = %d\n", errorCode);
        return errorCode;
    }
    printf("end time = %ld seconds, %d microseconds\n", endTime.tv_sec, endTime.tv_usec);
    
    int timeDiff = ((endTime.tv_sec * 1000) + endTime.tv_usec) - ((startTime.tv_sec * 1000) + startTime.tv_usec); // in microseconds
    float avgTime = (float) timeDiff / ITER_TIMES;
    printf("each system call time = %f microseconds\n", avgTime);
    
    return 0;
}