#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef int ll; 
#define PATH "serverSocketFile"
typedef struct fileDescriptorNode 
{
	int id,fd;
} fileDnode;

fileDnode clientDescriptionNodes[500];
ll clientCounter = 0;

void *handleThreadsForClients(void *args)
{
	fileDnode* inp = (fileDnode*)args;
	ll currentFileDescriptor = clientDescriptionNodes[clientCounter-1].fd;
	ll currentClientID = clientDescriptionNodes[clientCounter-1].id;
	ll done = 0,n;
	char stringRecieved[100];
	// printf("%d\n",currentClientID);
	while(1)
	{
		if((n=recv(currentFileDescriptor, stringRecieved, 200, 0))==-1)
		{
			perror("Error in message reception: ");
			break;
		}
		
		ll toBeSendClientID=atoi(strtok(stringRecieved," "));
		if(toBeSendClientID>200)
		{
			perror("error::\nuse ids between 0-200 (0 for broadcast)");
		}
		if( ( toBeSendClientID ) == 0)
		{
			if(send(currentFileDescriptor, "Invalid ID", 200, 0) >= 0)
			{
				printf("Receiving message from client\n");
			}
			else{
				perror("Invalid ID");
			}
		}
		else if(toBeSendClientID != -1)
		{
			toBeSendClientID -= 1;
			char* msg = strtok(NULL, "\n");
			printf("message recieved for Client:: %d -\n%s\n",toBeSendClientID+1,msg);
			if(msg == NULL || *msg == '\0')
			{
				if(send(currentFileDescriptor, "null Message", 200, 0) == -1)
				{
					perror("null Message");
					printf("\nby client: %d", toBeSendClientID);
				}
			}
			else
			{
				char sendMsg[300];
				sprintf(sendMsg, "Message Recieved - \n\tuser:%d \n\tmessage:%s\n", currentClientID+1, msg);
				if(toBeSendClientID == -1)
				{
					ll clnt = 0;
					while(clnt < clientCounter)
					{
						if(currentClientID == clientDescriptionNodes[clnt].id) continue;
						ll out = send(clientDescriptionNodes[clnt].fd, sendMsg, 200, 0);
						if(out==-1)
						{
							printf("there was error in sending the request\nmaybe user : %d left",clnt);
						}
						clnt++;
					}
				}
				else
				{
					ll out = send(clientDescriptionNodes[toBeSendClientID].fd, sendMsg, 200, 0);
					if(out==-1)
					{
						perror("Error in sending message/user left: ");
					}
				}
			}
		}
		else if(toBeSendClientID==-1){
			ll clnt = 0;
			char* msg = strtok(NULL, "\n");
			char sendMsg[300];
			//printf("Receiving message from client\n");
			printf("Broadcast Message Recieved - \n\tfrom client:%d \n\tmessage:%s\n", currentClientID+1 , msg);

			sprintf(sendMsg, "Broadcast Message Recieved - \n\tfrom user:%d \n\tmessage:%s\n", currentClientID+1 , msg);
			while(clnt < clientCounter)
			{
				if(clnt != currentClientID)
				{
					ll out = send(clientDescriptionNodes[clnt].fd, sendMsg, 200, 0);
					if(out==-1)
					{
						printf("there was error in sending the request\nmaybe user : %d left",clnt);
					}
				}

				
				clnt++;
			}
		}
	}

	close(currentFileDescriptor);
	return NULL;
}

int main(void)
{
	struct sockaddr_un local,remote;

	ll s = socket(AF_UNIX,SOCK_STREAM ,0);
	if(s==-1)
	{
		perror("Error in server connection\n");
		exit(1);
	}
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, "serverSocketFile");
	unlink(PATH);
	ll len = strlen("serverSocketFile") + sizeof(local.sun_family);
	if( (bind(s,(struct sockaddr *)&local, len+1))==-1) 
	{
		perror("binding error in server\n");
		exit(1);
	}
	if(listen(s,5) == -1)
	{
		printf("404: Unknown error.\n");
		return -1;
	}


	printf("Waiting for users to connect.\n");

	while(1)
	{
		ll t = sizeof(remote);
		ll fdr = accept(s,(struct sockaddr *)&remote, &t);
		fileDnode* inp = (fileDnode*)malloc(sizeof(fileDnode));
		inp->id = clientCounter;
		inp->fd = fdr;
		clientDescriptionNodes[clientCounter].id = clientCounter;
		clientDescriptionNodes[clientCounter].fd = fdr;
		printf("Connected with client id: %d\n", ++clientCounter);
		pthread_t thread1;
		pthread_create(&thread1, NULL, handleThreadsForClients, inp);
	}
	return 0;
}
