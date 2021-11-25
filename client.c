#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define PORTNUM 9004

char sbuf[256], rbuf[256];
int sd;
int rlen;

void *recv_func(void *arg){
	while(1){
		memset(rbuf, '\0', sizeof(rbuf));
		rlen = recv(sd, rbuf, sizeof(rbuf), 0);
		if(strncmp(rbuf, "exit", 4) == 0)
			exit(0);
		if(rlen > 0)
			printf("server : %s\n", rbuf);
	}
}

void *send_func(void *arg){
	while(1){
		memset(sbuf, '\0', sizeof(sbuf));
		printf("Enter message : %s\n", fgets(sbuf, sizeof(sbuf), stdin));
		sbuf[strlen(sbuf)] = '\0';
		if(send(sd, sbuf, strlen(sbuf)+1, 0) == -1){
			perror("send");
			exit(1);
		}
		if(strncmp(sbuf, "exit", 4) == 0)
			exit(0);
	}
}

int main(){
	struct sockaddr_in sin;
	pthread_t pth1, pth2;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sd, (struct sockaddr *)&sin, sizeof(sin))){
		perror("connet");
		exit(1);
	}

	printf("=====chatting start=====\n");

	pthread_create(&pth1, NULL, send_func, NULL);
	pthread_create(&pth2, NULL, recv_func, NULL);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);

	close(sd);
}
