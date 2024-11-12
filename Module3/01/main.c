#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int calculate(int i)
{
    return i * i;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
        exit(EXIT_FAILURE);

    int mid = (argc - 1) / 2 + 1;

    pid_t pid;
    switch (pid = fork())
    {
    case 0:
        for(int i = mid; i < argc; ++i)
            printf("Side: %d, area: %d\n", atoi(argv[i]), calculate(atoi(argv[i])));
        break;
    
    default:
        for(int i = 1; i < mid; ++i)
            printf("Side: %d, area: %d\n", atoi(argv[i]), calculate(atoi(argv[i])));
        break;
    }
}