#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    pid_t child_pid = fork();
    if(child_pid>=0)
    {
        if(child_pid==0)
        {
            execv("sequential_min_max",argv);
        }
    }
    wait(0);
    return 0;    
}