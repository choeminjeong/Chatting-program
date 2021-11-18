#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ncurse.h>

char *choices[] = {
	"GAME START",
	"GAME EXPLAIN",
	"GAME EXIT",
};

int n_choices = sizeof(choices)/sizeof(char*);
void print_menu(WINDOW *menu_win, int hightlight){
	box(menu_win, 0, 0);
	for(int i = 0; i < n_choices; ++i){
		if(highlight == i+1){
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void *game_thread(void *arg){
   	init_scr();
	clear();
	noecho();
	cbreak();

	while(1){
		if(start){
			sleep(0.5);
			if(fork() == -1){
				perror("fork");
				exit(1);
			}
		}
	}

	while(1){
		c = wgetch(menu_win);
		switch(c){
			case KEY_UP:
				row = -1;
				col = 0;
			case KEY_DOWN:
				row = -1;
				col = 0;
			case KEY_RIGHT:
				row = -1;
				col = 0;
			case KEY_LEFT:
				row = -1;
				col = 0;
		}
	}
}


void *explain_thread(void *arg){

	initscr();
	clear();
	noecho();
	explain_win = newwin(20, 10, 30, 30);
	mvprintw(
	mvprintw();
}

int main(int argc, char* argv[]){
	WINDOW *menu_win;
	WINDOW *explain_win;
	void *exarg = (void*)explain_win;
	int hightlight = 1;
	int choice = 0;
	int c;


	int sd, nd;
	struct sockaddr_in ser, cli;
	char buf[BUFSIZE];
	int slen = sizeof(ser);
	pthread_t sth, eth;
	void *message1, *message2;

	if(argc != 3){
		printf("enter error");
		exit(1);
	}
	
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	memset((char *) &ser, 0 slen);
	ser.sin_family = AF_INET;
	ser.sin_port = htons(argv[1]);
	ser.sin_addr.s_addr = inet_addr(argv[2]);

	if(bind(sd, (struct sockaddr_in*)&ser, slen) == -1){
		perror("bind");
		exit(1);
	}
	
	if(listen(sd, 5) == -1){
		perror("listen");
		exit(1);
	}
	
	while(1){
		int clen = sizeof(cli);
		if((nd = accept(sd, (struct sockaddr *)&cli, &clen)) == -1){
			perror("accpet");
			exit(1);
		}

		initscr();
		clear();
		noecho();
		cbreak();

		menu_win = newwin(30, 10, 0, 2);
		keypad(menu_win, TRUE);
		mvprintw(0, 0, "CHOOSE ONE SERVICE");
		refresh();
		print_menu(menu_win, highlight);
		while(1){
			c = wgetch(menu_win);
			switch(c){
				case KEY_UP:
					if(highlight == 1)
						highlight = n_choices;
					else
						--highlight;
					break;
				case KEY_DOWN:
					if(highlight == n_choices)
						highlight = 1;
					else
						++highlight;
					break;
				case KEY_ENTER:
					if(choice == choices[0]){
						mvprintw(1, 1, "GAME START");
						break;
					}else if(choice == choices[1]){
						explain_thread(&eth, NULL, explain_thread, (void*)exarg);
					}else if(choice == choices[2]){
						break;
					}
				case 10:
					choice = highlight;
					break;
				default:
					refresh();
					break;

			}
			print_menu(menu_win, highlight);
			if(choice != 0)
				break;
		}
		clrtoeol();
		refresh();
		endwin();
		printf("YOU CHOOSE CHOICE %d WITH CHOICE STRING%s\n", choice, choices[choice - 1]);
		return 0;

		/*
		memset(buf, '\0', BUFSIZE);
		sprintf(buf, "%s", "<CHOOSE THE SERVICE>\n1. GAME START\n2. GAME EXPLAIN\n3. GAME EXIT\n");
		if(send(nd, buf, sizeof(buf), 0) == -1){
			perror("send");
			exit(1);
		}
		printf("%s", buf);

		memset(buf, '\0', BUFSIZE);
		if(recv(nd, buf, sizeof(buf), 0) == -1){
			perror("recv");
			exit(1);
		}
		printf("CLIENT'S ANSWER: %s\n", buf);
		

		if(!strcmp(buf, "GAME START")){
			memset(buf, '\0', BUFSIZE);
			pthread_create(&sth, NULL, game_thread, (void *)&nd);
			pthread_join(sth, NULL);
		}else if(!strcmp(buf, "GAME EXPLAIN")){
			memset(buf, '\0', BUFSIZE);
			pthread_create(&eth, NULL, explain_thread, (void *)&nd);
			pthread_join(sth, NULL);
		}else if(!strcmp(buf, "GAME EXIT")){
			exit(1);
		}
		*/
	}
}



