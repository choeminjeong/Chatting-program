#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>

#define MAX_CNT 30

WINDOW *game_win;

//player
char *player = "P";
int startx = 0, starty = 0;
int height = 30, width = 30;
int col = 15;
int flag_P = 1;
int u_score = 0; //score

//star
int flag[MAX_CNT] = {0,};
int starx[MAX_CNT], stary[MAX_CNT];
int c = 0;

int ac_game_start();
void print_playground(WINDOW *game_win);
void init_setting();
void end_setting();
void create_star();
void input_process();
void move_star();
void user_score();

int ac_game_start(){
	int sc;
	init_setting();
	while(1) {
		if(flag_P){
			create_star();
			input_process();
		}
		move_star();
		print_playground(game_win);
		if(flag_P == 0){
			mvprintw(0, 0, "collision!!!! your score is %d", u_score);
			refresh();
			sleep(4);
			break;
		}
	}
	sc = u_score;
	endwin();
	return u_score;
}

void init_setting(){
	initscr();
	clear();
	noecho();
	curs_set(FALSE);
	cbreak();

	//game playground start position
	startx = 5;
	starty = 1;

	//random
	srand(time(NULL));

	game_win = newwin(height, width, starty, startx);
	keypad(game_win, TRUE);
	nodelay(game_win, TRUE);

	//print playground
	print_playground(game_win);
}

void print_playground(WINDOW *game_win)
{
	wclear(game_win);
	box(game_win, 0, 0);
	if(col < 1) col = 1;
	if(col > 28) col = 28;
	mvwprintw(game_win, height - 2, col, player);
	wrefresh(game_win);
	user_score();
}

void create_star(){
	for(int i = 0 ; i < MAX_CNT ; i++){
		if(!flag[i]){
			starx[i] = (rand() % 27) + 1;
			stary[i] = 1;
			flag[i] = 1;
			break;
		}
	}
}

void input_process(){
	c = wgetch(game_win);
	switch(c)
	{
		case KEY_LEFT:
			--col;
			break;
		case KEY_RIGHT:
			++col;
			break;
		default:
			break;
	}
	if(c == 'q'){
		end_setting();
		exit(0);
	}
}

void move_star(){
	for(int i = 0 ; i < MAX_CNT ; i++){
		if(flag[i]){
			mvwprintw(game_win, stary[i], starx[i], "s");
			if((stary[i] == (height - 2)) && (starx[i] == col)){
				//collision
				player = "X";
				flag_P = 0;
			}
			stary[i]++;
			if(stary[i] > (height - 2)){
				flag[i] = 0;
			}
		}
		wrefresh(game_win);
		usleep(10000);
	}
}

void end_setting(){
	curs_set(TRUE);
	clrtoeol();
	refresh();
	endwin();
}

void user_score(){
	if(flag_P){
		u_score++;
		mvprintw(0,0,"score : %d",u_score);
		refresh();
	}
}
