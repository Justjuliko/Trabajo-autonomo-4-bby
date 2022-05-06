#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* receive_msg(void *param)
{
    mqd_t mq1 = mq_open("/mq1", O_RDONLY);
    char buff[32];

    while (1)
    {
        mq_receive(mq1, buff, 32, NULL);
        printf("Message received: %s\n", buff);
        if( strncmp(buff, "exit", strlen("exit")) == 0){
            exit(EXIT_SUCCESS);
        }
    }
    return NULL;    
}

int main(int argc, char *argv[])
{
    mqd_t mq = mq_open("/mq0", O_WRONLY);
    char str[64];

    pthread_t threadID;
    pthread_create(&threadID,NULL,&receive_msg,NULL);

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
        mq_send(mq, str, strlen(str) + 1, 0);
        if (strncmp(str, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }

    mq_close(mq);
    exit(EXIT_FAILURE);
}