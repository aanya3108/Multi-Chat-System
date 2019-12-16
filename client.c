#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCK_PATH "serverSocketFile\0"
typedef int ll;
ll s;
typedef struct fileDescriptorNode 
{
	int id,fd;
} fileDnode;

void *messageSendingFunction(void *num)
{
	char messageString[100];
    ll i = 0;
    for(i =0 ; i < 1000 ; i++)
	{
        printf("To send someone a message, follow the following format\n");
        printf("[USER_ID] <space> [MESSAGE]\n");
        fgets(messageString,100,stdin);
        if(messageString[0] == '\0' || messageString[0] == '\n')
        {
            printf("write messages correctly\n");
            // exit(1);
        }
        else if(messageString[0] == 'q')
        {
            printf("Quiting the program.\n");
            exit(1);
        }
        ll temp = send(s, messageString, strlen(messageString), 0); 
        if(temp <= -1) 
        {
            perror("Error in sending: ");
            exit(1);
        }
    }
    exit(1);
}

void *messageRecievingFunction(void *num)
{
	while(1) 
	{
        ll recvMessageLen;
		char messageString[200];
        recvMessageLen = recv(s, messageString, 200, 0);
        if(recvMessageLen <= 0)
        {
            perror("Error in recieving data from server: \n");
            printf("Server closed connection\nClosing the application\n");
            exit(1);
        }
        messageString[recvMessageLen] = '\0';
        printf("\n%s\nSend message as [USERID] <space> [MESSAGE]:\n-> ", messageString);
    }
}
ll main(void)
{
    ll len;
    struct sockaddr_un remote;
    char str[100];
    s = socket(AF_UNIX, SOCK_STREAM , 0);
    if (s < 0) 
    {
        perror("Error in connection - client side: ");
        exit(1);
    }
    printf("Connecting to server\n");
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, "serverSocketFile\0");
    len = strlen("serverSocketFile\0") + sizeof(remote.sun_family);
    fileDnode* temp;
    ll con = connect(s, (struct sockaddr *)&remote, len); 
    if( con < 0) 
    {
        perror("Error in connection - server side: ");
        exit(1);
    }
    temp->fd = 0;
    printf("Connected.\n");
    temp->id = 1;
   	pthread_t thread1,thread2;
   	pthread_create(&thread1,NULL,messageSendingFunction,temp);
    printf("thread connected\n");
   	pthread_create(&thread2,NULL,messageRecievingFunction,temp);
    printf("joining threads\n");
   	pthread_join(thread1,NULL);
   	pthread_join(thread2,NULL);
    close(s);
    return 0;
}