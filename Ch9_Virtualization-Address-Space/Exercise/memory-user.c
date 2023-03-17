#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: ./memory-user [memory_size]\n");
        return -1;
    }
    
    printf("my PID = %d\n", (int)getpid());

    int memMegaByteSize = atoi(argv[1]); // mega bytes
    int memByteSize = memMegaByteSize * (int)pow(2, 10) * (int)pow(2, 10);
    char* memPtr = malloc(memByteSize);
    if (!memPtr) {
        printf("no available memory for allocating %d MB size memory\n", memMegaByteSize);
        return -1;

    }

    for (int i = 0; i < memByteSize; i++) {
        memPtr[i] = 0;
        if (i == memByteSize - 1) {
            i = 0;
        }
    }
    return 0;
}