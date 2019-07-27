
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
	char s[10000],s1[10000];
    char sendBuff[1025];
	int temp;
    time_t ticks; 
	
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 
	
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	
    listen(listenfd, 10); 
    char ch;
    while(true)
    {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		//printf("if want to exit at any time please write exit\n");
		while(1)
		{
		fflush(stdin);
		printf("waiting for client to write\n"); 
		read(connfd,s, 1000); 
		if(!strcmp(s,"exit"))
		{
			printf("\t\tserver says:okay Bye Bye\n");
			exit(0);
		}
		printf("\t\tclient said: %s\n", s);
		printf("server is writing\n");
		scanf(" %[^\t\n]s",s1);
		if(!strcmp(s1,"exit"))
		{
			printf("\t\tserver says:I am going off TATA.....\n");
			exit(0);
		}
		write(connfd, s1, 1000);
		fflush(stdin);
		}
		close(connfd);
    }
return 0;
}
