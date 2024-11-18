#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define MSG_SIZE 100

#define LOGIN_CODE 1
#define LOGIN_SUCC "login"
#define MSG_CODE 2
#define LOGOUT_CODE 3

#define MSG_SRV 1

#define MAX_USERS 10

struct messagestr
{
    long mtype;
    char mtext[MSG_SIZE];
};

int main()
{
    int user[MAX_USERS];
    int counter = 0;

    key_t key = ftok(".queuef", 'a');
    if (key  == -1)
    {
        perror("Ошибка преобразования в key_t");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, S_IRWXU | S_IRWXG | IPC_CREAT) ;
    if(msgid == -1)
    {
        perror("Ошибка создания очереди сообщений");
        exit(EXIT_FAILURE);
    }

    struct messagestr message;
    while (1)
    {
        if (msgrcv(msgid, &message, sizeof(message.mtext), MSG_SRV, 0) == -1)
        {
            perror("Ошибка получения сообщения");
            exit(EXIT_FAILURE);
        }
        
        switch (message.mtext[0])
        {
        case LOGIN_CODE:
            if(counter < MAX_USERS)
            {
                user[counter] = atoi(&message.mtext[1]);
                message.mtype = user[counter];
                strcpy(message.mtext, LOGIN_SUCC);
                msgsnd(msgid, &message, sizeof(message.mtext), 0);
                printf("Used logged in. Pid: %d\n", user[counter]);
                ++counter;
            }
            else
            {
                message.mtype = atoi(&message.mtext[1]);
                strcpy(message.mtext, "");
                printf("User login failed. Pid: %d\n", atoi(&message.mtext[1]));
                msgsnd(msgid, &message, sizeof(message.mtext), 0);
            }
            break;
        
        case MSG_CODE:
        {
            char* s = strtok(&message.mtext[1], "\30");
            pid_t tmppid = atoi(s);
            s = strtok(NULL, "|");
            printf("Новое сообщение пользователям: %s", s);
            sprintf(message.mtext, "%c%s", MSG_CODE, s);
            for(int i = 0; i < counter; ++i)
            {
                if (user[i] != tmppid)
                {
                    message.mtype = user[i];
                    msgsnd(msgid, &message, sizeof(message.mtext), 0);
                }
            }
            break;
        }

        case LOGOUT_CODE:
        {
            int flag = 0;
            for(int i = 0; i < counter; ++i)
            {
                if (user[i] == atoi(&message.mtext[1]))
                {
                    flag = 1;
                    ++i;
                }

                if (flag)
                    user[i - 1] = user[i];
            }
            --counter;

            printf("User logged out. Pid: %d\n", atoi(&message.mtext[1]));
            fflush(stdout);
            break;
        }

        default:
            break;
        }
    }

    msgctl(msgid, IPC_RMID, NULL);
    
    exit(EXIT_SUCCESS);
}
