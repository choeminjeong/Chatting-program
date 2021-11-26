#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

#define PORTNUM 9004

char rbuf[256], sbuf[256];
struct sockaddr_in sin, cli;
int sd, ns, clientlen = sizeof(cli);
int rlen;

void *recv_func(void *arg){
	while(1){
		memset(rbuf, '\0', sizeof(rbuf));
		rlen = recv(ns, rbuf, sizeof(rbuf), 0);
		if(strncmp(rbuf, "exit", 4) == 0)
			exit(0);
		if(rlen > 0)
			printf("client: %s\n", rbuf);
		}
}

void *send_func(void *arg){
	while(1){
		memset(sbuf, '\0', sizeof(sbuf));
		printf("Enter message: %s\n", fgets(sbuf, sizeof(sbuf), stdin));
		sbuf[strlen(sbuf)] = '\0';
		if(send(ns, sbuf, strlen(sbuf)+1, 0) == -1){
			perror("send");
			exit(1);
		}
		if(strncmp(sbuf, "exit", 4) == 0)
			exit(0);
	}
}

int main(){
	char menu[256] = {0,};
	pthread_t pth1, pth2;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ //tcp socket
		perror("socket");
		exit(1);
	}

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
		perror("bind");
		exit(1);
	}

	if(listen(sd, 5)){ //queue size: 5
		perror("listen");
		exit(1);
	}

	if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1){
		perror("accept");
		exit(1);
	}

	memset(menu, '\0', sizeof(menu));
	sprintf(menu, "%s", "=====Available services=====\n1. Chatting\n2. Game1\n\
3. Game2\nEnter the service number\n");
	//Send a list of available services to the client
	send(ns, menu, strlen(menu)+1, 0);

	memset(menu, '\0', sizeof(menu));
	//Receive client's desired service number
	recv(ns, menu, sizeof(menu), 0);
	//Message output received from client
	printf("client selected %s\n", menu);


	if(!strncmp(menu, "1", 1)){ //chatting program
		printf("=====chatting start=====\n");

		pthread_create(&pth1, NULL, send_func, NULL);
		pthread_create(&pth2, NULL, recv_func, NULL);

		pthread_join(pth1, NULL);
		pthread_join(pth2, NULL);
	}
	if(!strncmp(menu, "2", 1)){ //game1
		printf("=====game 1=====\n");

		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}
	if(!strncmp(menu, "3", 1)){ //game2
		printf("=====game 2=====\n");

		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}

	close(ns);
	close(sd);

	return 0;

}
