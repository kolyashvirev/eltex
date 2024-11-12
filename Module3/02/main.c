#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
    char input[100];
    pthread_t thread;

    while(1)
    {
        printf("Введите команду: ");
        fgets(input, 100, stdin);
        input[strcspn(input, "\n")] = 0;

        if(strcmp(input, "exit") == 0)
            exit(EXIT_SUCCESS);

        
        char to[100];
        strcpy(to, input);
        pid_t pid;
        int status;
        switch (pid = fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;

        case 0:
        {
            int i = 0;
            char* args[10];
            char* curr = strtok((char*)input, " ");
            while(curr != NULL)
            {
                args[i++] = curr;
                curr = strtok(NULL, " ");
            }
            args[i] = NULL;

            char p[100];
            snprintf(p, sizeof(p), "./%s", args[0]);
            execv(p, args);

            perror("Ошибка выполнения команды.\n");
            exit(EXIT_FAILURE);
            break;
        }
        default:
            wait(NULL);
        }   
    }
}