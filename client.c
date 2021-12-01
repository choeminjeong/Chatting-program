#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>

#define PORTNUM 9004
#define HEIGHT 15
#define WIDTH 55

char sbuf[256], rbuf[256];
int sd;
int rlen;
int r = 2, c = 2;
int cnt = 0;

void *recv_func(void *arg){
	while(1){
		memset(rbuf, '\0', sizeof(rbuf));
		rlen = recv(sd, rbuf, sizeof(rbuf), 0);
		if(strncmp(rbuf, "exit", 4) == 0)
			exit(0);
		if(rlen > 0){
			//printf("server: %s", rbuf);	
	
			if(r >= 14){
				r = 1;
				clear();
			}
			move(r++, c);
			printw("server: ");
			printw("%s", rbuf);
			//char *a = (char *)arg;
			//mvwprintw((WINDOW *)arg, 1, 1, "hello");
			refresh();
		}
		
	}
}

void *send_func(void *arg){
	while(1){
		memset(sbuf, '\0', sizeof(sbuf));
		//printf("Enter message: %s\n", fgets(sbuf, sizeof(sbuf), stdin));
		
		move(30, 2);
		//printf("client: ");
		//scanf("%s", sbuf);
		//printf("%s", sbuf);
		printw("client: ");
		scanw("%s", sbuf);
		sbuf[strlen(sbuf)] = '\0';
		
		//printw("%s", sbuf);
		//
		
		if(send(sd, sbuf, strlen(sbuf), 0) == -1){
			perror("send");
			exit(1);
		}
		
		if(r >= 14){
			r = 2;
			clear();
		}
		move(r++, c);
		//printw("client: ");
		//printw("%s", sbuf);
		refresh();
		if(strncmp(sbuf, "exit", 4) == 0)
			exit(0);
	}
}

int startx = 0;
int starty = 0;

int main(){
	char menu[256] = {0,};
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

	memset(menu, '\0', sizeof(menu));
	//Receive a list of available services from server
	recv(sd, menu, sizeof(menu), 0);
	printf("%s", menu);

	memset(menu, '\0', sizeof(menu));
	printf("Enter the number: ");
	fgets(menu, sizeof(menu), stdin);
	//Send client's desired service number
	send(sd, menu, strlen(menu)+1, 0);

	if(!strncmp(menu, "1", 1)){
		
		initscr();
		cbreak;
		echo();
		curs_set(0);

		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		bkgd(COLOR_PAIR(1));
		
		
		WINDOW *win = newwin(HEIGHT, WIDTH, starty, startx);
		refresh();
		mvwprintw(win, 1, 1, "======chatting start==============\n");
		//printf("=====chatting start=====\n");

		box(win, 0, 0);
		scrollok(win, TRUE);
		wrefresh(win);

		//WINDOW *subwin = newwin(HEIGHT, WIDTH, starty, startx);
		//refresh();

		//box(subwin, 30, 30);
		//wrefresh(subwin);


		//void *arg = win;
		pthread_create(&pth1, NULL, send_func, NULL);
		//wrefresh(win);
		pthread_create(&pth2, NULL, recv_func, NULL);
		//wrefresh(win);

		pthread_join(pth1, NULL);
		pthread_join(pth2, NULL);
	}
	if(!strncmp(menu, "2", 1)){
		printf("=====game 1=====\n");

		while(1){
			memset(sbuf, '\0', sizeof(sbuf));
			printf("rock, sissor, paper: ");
			scanf("%s", sbuf);
			
			if(send(sd, sbuf, strlen(sbuf)+1, 0) == -1){
				perror("send");
				exit(1);
			}
			
			memset(rbuf, '\0', sizeof(rbuf));
			rlen = recv(sd, rbuf, sizeof(rbuf), 0);
			printf("SERVER: %s\n", rbuf);

			if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "sissor"))) || ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "paper"))) || ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "rock")))){
			 	printf("=====CLIENT WIN!=====\n");
				sleep(1);
				exit(1);
			}else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "paper"))) || ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "rock"))) || ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "sissor")))){
			 	printf("=====CLIENT LOSE!=====\n");
				sleep(1);
				exit(1);
			}else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "rock"))) || ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "sissor"))) || ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "paper")))){

				printf("=====EVEN: PLAY AGAIN!=====\n");
				sleep(1);
			}else{
				printf("=====THAT IS NOT CORRECT RSP!=====\n");
				sleep(1);
			}
		}
		sleep(1);
		close(sd);
	}
	if(!strncmp(menu, "3", 1)){
		printf("=====game 2=====\n");

		sleep(1);
		close(sd);
	}
	endwin();
	close(sd);
}

