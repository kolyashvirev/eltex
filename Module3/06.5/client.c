#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define MSG_SIZE 100
#define MSG_TEXT_SIZE 100


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
    pid_t mypid = getpid();
    char mypidchar[10];
    sprintf(mypidchar, "%d", mypid);
    printf("My pid: %d\n", mypid);

    int user[MAX_USERS];
    int counter = 0;

    key_t key = ftok(".queuef", 'a');
    if (key  == -1)
    {
        perror("Ошибка преобразования в key_t");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, S_IRWXU | S_IRWXG /*| S_IRWXO*/ | IPC_CREAT) ;
    if (msgid == -1)
    {
        perror("Ошибка создания очереди сообщений");
        exit(EXIT_FAILURE);
    }

    struct messagestr message;

    sprintf(message.mtext, "%c%s", LOGIN_CODE, mypidchar);
    message.mtype = MSG_SRV;
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) perror("login");

    if (msgrcv(msgid, &message, sizeof(message.mtext), mypid, 0) == -1)
    {
        perror("Ошибка авторизации");
        exit(EXIT_FAILURE);
    }
    if (strcmp(message.mtext, LOGIN_SUCC))
    {
        perror("Ошибка авторизации");
        exit(EXIT_FAILURE);      
    }
    printf("Успешная авторизация!\n");

    pid_t pid;
    switch (pid = fork())
    {
        case -1:
            perror("");
            break;
        
        case 0:
        {
            while (1)
            {
                if (msgrcv(msgid, &message, sizeof(message.mtext), mypid, 0) == -1)
                {
                    perror("Ошибка получения сообщения");
                    exit(EXIT_FAILURE);
                }
                printf("\nНовое сообщение: %s", &message.mtext[1]);
                printf("Введите сообщение: ");
                fflush(stdout);
            }
        }

        default:
        {
            while(1)
            {
                printf("Введите сообщение: ");
                char msg[MSG_SIZE - 3];
                fgets(msg, MSG_SIZE - 3, stdin);
                message.mtype = MSG_SRV;

                if (strcmp(msg, "exit\n") == 0 || 
                    strcmp(msg, "\n")     == 0)
                {
                    sprintf(message.mtext, "%c%s", LOGOUT_CODE, mypidchar);
                    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
                    {
                        perror("Ошибка отправки сообщения");
                        exit(EXIT_FAILURE);
                    }

                    break;
                }

                sprintf(message.mtext, "%c%d\30%s", MSG_CODE, mypid, msg);
                if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
                {
                    perror("Ошибка отправки сообщения");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
    }
    
    exit(EXIT_SUCCESS);
}
