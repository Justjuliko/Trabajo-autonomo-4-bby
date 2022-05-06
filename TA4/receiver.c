#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* send_msg(void *param)
{
    mqd_t mq1 = mq_open("/mq1", O_WRONLY);
    char str[64];

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
        mq_send(mq1, str, strlen(str) + 1, 0);
        if (strncmp(str, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    mqd_t mq;
    mqd_t mq1;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    struct mq_attr attr1;
    attr1.mq_flags = 0;
    attr1.mq_maxmsg = 10;
    attr1.mq_msgsize = 32;
    attr1.mq_curmsgs = 0;

    mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);
    mq1 = mq_open("/mq1", O_RDONLY | O_CREAT, 0644, &attr1);
    char buff[32];

    pthread_t threadID;
    pthread_create(&threadID,NULL,&send_msg,NULL);

    while (1)
    {
        mq_receive(mq, buff, 32, NULL);
        printf("Message received: %s\n", buff);
        if( strncmp(buff, "exit", strlen("exit")) == 0){
            break;
        }
    }

    mq_close(mq);
    mq_unlink("/mq0");
    exit(EXIT_SUCCESS);
}