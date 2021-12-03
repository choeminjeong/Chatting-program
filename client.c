#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "avoidCharGame.h"

#define PORTNUM 9009
#define WIDTH 30
#define HEIGHT 10
#define BLUE 4;

//rush dog
#define DOG_Y 15
#define DOG_X 20
#define POOP_Y 20
#define POOP_X 40
bool jump;

char sbuf[256], rbuf[256];
int sd;
int rlen;

//menu window
int s_x = 0;
int s_y = 0;
char *choices[] = {
			"Chatting",
			"Rock Sissor Paper",
			"Baskin Robbins",
			"Sipping",
			"Avoid Char",
			"Rush Dog"
		  };
WINDOW *menu_win;
int highlight = 1;
int choice = 0;
int c;
int n_choices = sizeof(choices) / sizeof(char *);

//menu window function
void start_menu();
void print_menu(WINDOW *menu_win, int highlight, int c);

//chatting function
void *recv_func();
void *send_func();

//rush dog function
void dog_shape(int x, int y);
void poop_shape(int x, int y);

int main()
{//start main
    struct sockaddr_in sin;
    pthread_t pth1, pth2;
	char menu[256] = {0,};

	//socket
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
	//

	start_menu();

	memset(menu, '\0', sizeof(menu));
	if(choice == 1){ //start choice 1
		strcpy(menu, "1");
		send(sd, menu, strlen(menu)+1, 0);
		printf("=====chatting start=====\n");

        pthread_create(&pth1, NULL, send_func, NULL);
        pthread_create(&pth2, NULL, recv_func, NULL);

        pthread_join(pth1, NULL);
        pthread_join(pth2, NULL);
	} //end choice 2
	if(choice == 2){ //start choice 2
		strcpy(menu, "2");
		printf("%s\n", menu);
		send(sd, menu, strlen(menu)+1, 0);
		printf("=====RSP game=====\n");
		while(1){ //start while
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

            if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "sissor"))) || \
            ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "paper"))) || \
            ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "rock")))){
                printf("=====CLIENT WIN!=====\n");
                sleep(1);
                exit(1);
            }else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "paper"))) || \
            ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "rock"))) || \
            ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "sissor")))){
                printf("=====CLIENT LOSE!=====\n");
                sleep(1);
                exit(1);
            }else if((!(strcmp(sbuf, "rock")) && !(strcmp(rbuf, "rock"))) || \
            ((!strcmp(sbuf, "sissor")) && !(strcmp(rbuf, "sissor"))) || \
            ((!strcmp(sbuf, "paper")) && !(strcmp(rbuf, "paper")))){

                printf("=====EVEN: PLAY AGAIN!=====\n");
                sleep(1);
            }else{
                printf("=====THAT IS NOT CORRECT RSP!=====\n");
                sleep(1);
            }
        } //end while
        sleep(1);
        close(sd);
		return 0;
	} //end choice 2
	if(choice == 3){ //start choice 3 baskin robbins
		strcpy(menu, "3");
		send(sd, menu, strlen(menu)+1, 0);
		initscr();
        start_color();
		init_pair(1,COLOR_RED,COLOR_YELLOW);
		init_pair(2,COLOR_BLUE,COLOR_BLACK);
		init_pair(4,COLOR_YELLOW,COLOR_BLACK);
		init_pair(5,COLOR_WHITE,COLOR_BLACK);
        int player=0;int computer=0;
        int num=0;
        srand((unsigned)time(NULL));
		attron(COLOR_PAIR(1));
        printw("------------baskinrobbins31 game-----------\n");
		attroff(COLOR_PAIR(1));
		refresh();
		getch();
		endwin();
		init_pair(3,COLOR_GREEN,COLOR_BLACK);
		attron(COLOR_PAIR(3));
        printw("GAME START");
		attroff(COLOR_PAIR(3));
		refresh();
		attron(COLOR_PAIR(2));
		initscr();
        printw("**Enter the number from 1 to 3**\n");
		initscr();
		attroff(COLOR_PAIR(2));
		initscr();
		refresh();
        while(1){
			refresh();

			init_pair(5,COLOR_WHITE,COLOR_BLACK);
			attron(COLOR_PAIR(5));
	        printw("\n[player] how many numbers will you enter? : ");
			attroff(COLOR_PAIR(5));
			refresh();
	        scanf("%d",&player);
	        if(player>3 || player<=0){
	            printf("Only enter numbers from 1 to 3\n\n");
	            continue;
	        }
	        for(int i=1;i<=player;i++){
	            num++;
	            if(num<=31){
	                printf("%d",num);
	            }
	        }
	        if(num>=31){
	            printf("\n*** computer win! ***");
	            break;
	        }
	        computer = (rand()%3+1);
	        printf("\n[computer]: %d\n",computer);

	        for(int i=0;i<computer;i++){
	            num++;
	            if(num<=31){
	                printf("%d ",num);
	            }
        	}
	        if(num>=31){
	            printf("\n*** player win! ***\n");
	            break;
        	}
		}
		return 0;
	}
	if(choice == 4){
		initscr();
		clear();
		echo();
		curs_set(TRUE);

		strcpy(menu, "4");
		send(sd, menu, strlen(menu)+1, 0);
		mvprintw(0,0,"=====Sipping Game=====\n");

		//Receive random number from server
        memset(rbuf, '\0', sizeof(rbuf));
        recv(sd, rbuf, sizeof(rbuf), 0);

		mvprintw(1,0,"I received a number from the server.\n");
		mvprintw(2,0,"Is this an odd number or an even number?(odd, even)\n");

		memset(sbuf, '\0', sizeof(sbuf));
		//printf("Enter(odd or even): %s", fgets(sbuf, sizeof(sbuf), stdin));
		mvprintw(3,0,"Enter(odd or even): ");
		scanw("%s", sbuf);
		send(sd, sbuf, sizeof(sbuf), 0);

		printf("recv: %s, send: %s\n", rbuf, sbuf);

		if(!strcmp(rbuf, sbuf) == 0){
            mvprintw(5,0,"=====CLIENT WIN!=====\n");
		}else{
            mvprintw(5,0,"=====CLIENT LOSE!=====\n");
		}

		sleep(5);
		close(sd);
		clear();
		endwin();
		return 0;
	}
	if(choice == 5){
		//avoid char game
		strcpy(menu, "5");
		send(sd, menu, strlen(menu)+1, 0);

		int score = ac_game_start();
		memset(sbuf, '\0', sizeof(sbuf));
		sprintf(sbuf, "%d", score);
		send(sd, sbuf, strlen(sbuf)+1, 0);
	}
	if(choice == 6){
		//rush dog game
		strcpy(menu, "6");
		send(sd, menu, strlen(menu)+1, 0);

		initscr();
		int ch;

		while(1){
			int dog_y = DOG_Y;
			int dog_x = DOG_X;
			int poop_y = POOP_Y;
			int poop_x = POOP_X;
			int score = 0;
			jump = false;
			bool bottom = true;
			time_t ttime = time(NULL);
			int jump_y = dog_y-5;

			while(1){
				nodelay(stdscr, TRUE);
				ch = getch();
				if(ch == ' ' && bottom){
					if(poop_y == POOP_Y){
						if(dog_y <= jump_y){
							break;
						}
						jump = true;
						bottom = false;
					}
				}

				poop_x -= 2;
				if(poop_x <= 0){
					poop_x = POOP_X;
				}
				clear();

				if(jump){
					dog_y--;
				}else{
					dog_y++;
				}

				if(dog_y >= 16){
					dog_y = 16;
					bottom = true;
				}

				if(dog_y <= 11){
					jump = false;
				}

				dog_shape(0, dog_y);
				poop_shape(poop_x, POOP_Y);

				if(poop_x <= 8 && poop_x >= 7){
					refresh();
					if(!jump){
						break;
					}
				}

				if(poop_x == dog_x){
					ttime = time(NULL);
					score++;
				}

				mvprintw(0, 0, "------------------------------PRESENT SCORE: %d---------------------------------", score);
				mvprintw(23, 0, "-------------------------------------------------------------------------------");
				mvprintw(2, 0, "+       +       +       +       +       +");
				mvprintw(3, 0, "    +       +       +       +       +       +                        +      +      +      +");
				mvprintw(4, 0, "+       +       +       +       +       +                        +      +      +      +");
				mvprintw(5, 0, "    +       +       +       +       +       +                        +      +      +      +");
				mvprintw(6, 0, "+       +       +       +       +       +                        +      +      +      +");
				mvprintw(7, 0, "    +       +       +       +       +       +       +       +       +       +");
				mvprintw(8, 0, "+       +       +       +       +       +       +       +       +       +");
				mvprintw(9, 0, "    +       +       +       +       +       +       +       +       +       +");
				mvprintw(10, 0, "+       +       +       +       +       +       +       +       +       +");
				mvprintw(11, 0, "    +       +       +       +       +       +       +       +       +       +");
				mvprintw(2, 40, "+++JumpKey: <space>+++");
				mvprintw(3, 40, "|  Jump              |");
				mvprintw(4, 40, "|      and           |");
				mvprintw(5, 40, "|         avoid      |");
				mvprintw(6, 40, "|              poop  |");
				mvprintw(7, 40, "++++++++++++++++++++++");
				refresh();
				napms(1000/15);
			}
			mvprintw(0, 0, "-------------------------------------------------------------------------------");
			mvprintw(3, 27, " ____________________\n");
			mvprintw(4, 27, "|                    |\n");
			mvprintw(5, 27, "|        GAME        |\n");
			mvprintw(6, 27, "|        OVER        |\n");
			mvprintw(7, 27, "|____________________|\n");
			mvprintw(9, 29, "[FINAL SCORE: %d]", score);
			mvprintw(11, 0, "-------------------------------------------------------------------------------");
			mvprintw(13, 13, ">> PRESS key 'q': the game will be terminated.");
       		mvprintw(14, 13, ">> PRESS 'any key' except 'q': the game will be restarted.");
			mvprintw(21, 15, "<<----------------dog touched poop!!");
			memset(sbuf, '\0', sizeof(sbuf));
			sprintf(sbuf, "%d", score);
			if(send(sd, sbuf, strlen(sbuf)+1, 0) == -1){
				perror("send");
				exit(1);
			}

			refresh();
			nodelay(stdscr, FALSE);
			ch = getch();
			if(ch == 'q'){
				break;
			}
		}
		endwin();
		sleep(1);
		close(sd);
	}
	sleep(1);
	close(sd);
	return 0;
}

void start_menu(){
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (80 - WIDTH) / 2;
	starty = (40 - HEIGHT) / 2;
	curs_set(FALSE);

	start_color();
	init_pair(1, 4, 0);

	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	mvprintw(4, 15, "                         o     o ooooo    ooo   o     o\n");
	mvprintw(5, 15, "                          o   o  o    o  o   o   o   o\n");
	mvprintw(6, 15, "o   o ooo o   o ooo        o o   o    o o     o   o o\n");
	mvprintw(7, 15, "oo oo  o  oo  o  o          o    ooooo  o     o    o\n");
	mvprintw(8, 15, "o o o  o  o o o  o   ---   o o   o    o o     o   o o\n");
	mvprintw(9, 15, "o   o  o  o  oo  o        o   o  o    o  o   o   o   o\n");
	mvprintw(10, 15,"o   o ooo o   o ooo      o     o ooooo    ooo   o     o\n");
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	refresh();
	print_menu(menu_win, highlight, c);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
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
			case KEY_ENTER: // enter key at the key pad
			case 10: // enter key
				choice = highlight;
				break;
			default:
				mvprintw(1, 0, "(Charcter pressed = %3d or '%c')", c, c);
				refresh();
				break;
		}
		print_menu(menu_win, highlight, c);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}
	clrtoeol();
	refresh();
	move(0,0);
	clear();
	wclear(menu_win);
//	wendwin(menu_win);
	endwin();

}

void print_menu(WINDOW *menu_win, int highlight, int c)
{
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void *recv_func(void *arg){
    while(1){
        memset(rbuf, '\0', sizeof(rbuf));
        rlen = recv(sd, rbuf, sizeof(rbuf), 0);
        if(strncmp(rbuf, "exit", 4) == 0){
            close(sd);
            exit(0);
        }
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
        if(strncmp(sbuf, "exit", 4) == 0){
            close(sd);
            exit(0);
        }
    }
}

void dog_shape(int x, int y){
	move(y, x);
	if(!jump){
		printw("o      oooo\n");
		printw("oo    oooooo\n");
    		printw("ooooooooooo\n");
   		printw(" oooooooooo\n");
		printw(" ooo   ooo\n");
		printw("  ooo  oo\n");
		printw("   oo  oo\n");
	}else{
		printw("   o   oooo\n");
		printw(" oo   oooooo\n");
    	printw("ooooooooooo\n");
   		printw(" oooooooooo\n");
		printw(" ooo   ooo\n");
		printw("oo      oo\n");
		printw("oo       oo\n");
	}
}

void poop_shape(int x, int y){
	mvprintw(y++, x, "  o \n");
	mvprintw(y++, x, " ooo \n");
	mvprintw(y++, x, "ooooo\n");
}

