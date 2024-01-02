#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char* argv[])
{
    int x = 2;
    int pid = fork();

    if (pid == -1) {
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        x++;
    }
    sleep(2);
    printf("Value of x: %d\n", x);
    
    printf("Process id %d\n", getpid());
    if (pid != 0) {
        wait(NULL);
    }
    return EXIT_SUCCESS;
}