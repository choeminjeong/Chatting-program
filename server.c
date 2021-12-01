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

#define PORTNUM 9000
#define HEIGHT 15
#define WIDTH 55

char rbuf[256], sbuf[256];
int sd, ns;
int rlen;
int r = 2, c = 2;
int cnt = 0;

void *recv_func(void *arg){
	while(1){
		memset(rbuf, '\0', sizeof(rbuf));
		rlen = recv(ns, rbuf, sizeof(rbuf), 0);
		if(strncmp(rbuf, "exit", 4) == 0)
			exit(0);
		if(rlen > 0){
			//printf("client: %s\n", rbuf);
				
			if(r >= 14){
				r = 1;
				clear();
			}
			move(r++, c);
			printw("client: ");
			printw("%s", rbuf);
			refresh();
		}

	}

}

void *send_func(void *arg){
	while(1){
		memset(sbuf, '\0', sizeof(sbuf));
		printw("server: ");
		scanw("%s", sbuf);
		sbuf[strlen(sbuf)] = '\0';
		if(send(ns, sbuf, strlen(sbuf), 0) == -1){
			perror("send");
			exit(1);
		}
		if(r >= 14){
			r = 2;
			clear();
		}
		move(r++, c);
		refresh();

		if(strncmp(sbuf, "exit", 4) == 0)
			exit(0);
	}
}

int startx = 0;
int starty = 0;

int main(){
	char menu[256] = {0,};
	pthread_t pth1, pth2;
	struct sockaddr_in sin, cli;
	int clientlen = sizeof(cli);
	
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
		
		initscr();
		cbreak;
		echo();
		curs_set(0);

		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		bkgd(COLOR_PAIR(1));

		WINDOW *win = newwin(HEIGHT, WIDTH, starty, startx);
		refresh();
		mvwprintw(win, 1, 1, "======chatting start============\n");

		box(win, 0, 0);
		scrollok(win, TRUE);
		wrefresh(win);

		pthread_create(&pth1, NULL, send_func, NULL);
		pthread_create(&pth2, NULL, recv_func, NULL);

		pthread_join(pth1, NULL);
		pthread_join(pth2, NULL);
	}
	if(!strncmp(menu, "2", 1)){ //game1
		printf("=====game 1=====\n");
		
		while(1){
			memset(sbuf, '\0', sizeof(sbuf));
			printf("rock, sissor, paper: ");
			scanf("%s", sbuf);
			
			memset(rbuf, '\0', sizeof(rbuf));
			rlen = recv(ns, rbuf, sizeof(rbuf), 0);
			printf("CLIENT: %s\n", rbuf);


			if(send(ns, sbuf, strlen(sbuf)+1, 0) == -1){
				perror("send");
				exit(1);
			}
			
			if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "sissor"))) || (!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "paper"))) || (!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "rock")))){
				printf("=====SERVER WIN!=====\n");
				sleep(1);
				close(ns);
				close(sd);
				exit(1);
			}else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "paper"))) || (!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "rock"))) || (!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "sissor")))){
				printf("=====SERVER LOSE!=====\n");
				sleep(1);
				close(ns);
				close(sd);
				exit(1);
			}else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "rock"))) || (!(strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "sissor"))) || (!(strcmp(sbuf, "paper")) && !(strcmp(rbuf, "paper")))){
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
