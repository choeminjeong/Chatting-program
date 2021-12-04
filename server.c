#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <ncurses.h>
#include <time.h>

#define PORTNUM 9031

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
	//Receive client's desired service number
	recv(ns, menu, sizeof(menu), 0);
	//Message output received from client
	printf("client choose service %s\n", menu);

	if(!strncmp(menu, "1", 1)){ //chatting program
		printf("=====chatting start=====\n");

		pthread_create(&pth1, NULL, send_func, NULL);
		pthread_create(&pth2, NULL, recv_func, NULL);

		pthread_join(pth1, NULL);
		pthread_join(pth2, NULL);
	}
	if(!strncmp(menu, "2", 1)){ //RSP
		printf("=====RSP game=====\n");
			while(1){
				memset(rbuf, '\0', sizeof(rbuf));
				rlen = recv(ns, rbuf, sizeof(rbuf), 0);
				printf("CLIENT: %s\n", rbuf);

				memset(sbuf, '\0', sizeof(sbuf));
				printf("rock, sissor, paper: ");
				scanf("%s", sbuf);

				if(send(ns, sbuf, strlen(sbuf)+1, 0) == -1){
					perror("send");
					exit(1);
				}

				if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "sissor"))) || \
				(!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "paper"))) || \
				(!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "rock")))){
					printf("=====SERVER WIN!=====\n");
					sleep(1);
					close(ns);
					close(sd);
					exit(1);
				}
				else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "paper"))) || \
				(!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "rock"))) || \
				(!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "sissor")))){
					printf("=====SERVER LOSE!=====\n");
					sleep(1);
					close(ns);
					close(sd);
					exit(1);
				}else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "rock"))) || \
				(!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "sissor"))) || \
				(!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "paper")))){
					printf("=====EVEN: PLAY AGAIN!=====\n");
					sleep(1);
				}else{
					printf("=====THAT IS NOt CORRECT RSP!=====\n");
					sleep(1);
				}
			}
		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}
	if(!strncmp(menu, "3", 1)){ //Baskin Robbins
		printf("=====Baskin Robbins Game=====\n");

		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}
	if(!strncmp(menu, "4", 1)){ //Sipping Game
		printf("=====Sipping Game=====\n");
		int random = 0;

		srand(time(NULL));
		random = rand() % 2;

		printf("random number: %d\n", random);

		memset(sbuf, '\0', sizeof(sbuf));
		if(random == 0){ //even number
			strcpy(sbuf, "even");
		}else{
			strcpy(sbuf, "odd");
		}
		send(ns, sbuf, strlen(sbuf)+1, 0);

		memset(rbuf, '\0', sizeof(rbuf));
		recv(ns, rbuf, sizeof(rbuf), 0);

		printf("recv: %s, send: %s\n", rbuf, sbuf);

		if(strcmp(rbuf, sbuf) == 0){
            printf("=====CLIENT WIN!=====\n");
        }else{
            printf("=====CLIENT LOSE!=====\n");
        }

		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}
	if(!strncmp(menu, "5", 1)){ //Avoid Char Game
		printf("=====Avoid Char Game=====\n");

		memset(rbuf, '\0', sizeof(rbuf));
		recv(ns, rbuf, sizeof(rbuf), 0);
		printf("client's score is %s\n", rbuf);

		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}
	if(!strncmp(menu, "6", 1)){ //Rush Dog
		printf("=====Rush Dog Game=====\n");

		while(1){
			memset(rbuf, '\0', sizeof(rbuf));
			recv(ns, rbuf, sizeof(rbuf), 0);
			if(!strcmp(rbuf, "q")){
				break;
			}
			printf("client's score is %s\n", rbuf);
		}
		sleep(1); //bind error prevention
		close(ns);
		close(sd);
		return(0);
	}


	close(ns);
	close(sd);

	return 0;

}

