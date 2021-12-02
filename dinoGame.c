#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define DOG_Y 18
#define DOG_X 20
#define POOP_Y 22 
#define POOP_X 40

bool jump;

int getKeyNoWait(){
	nodelay(stdscr, TRUE);
	return getch();
}

int getKeyWait(){
	nodelay(stdscr, FALSE);
	return getch();
}

void dog_shape(int x, int y){
	move(y, x);
	printw("o     ooo\n");
	printw("o    ooooo\n");
	printw("ooooooooo\n");
   	printw(" oooooooo\n");
	if(jump){
		printw(" oo   oo\n");
		printw("oo     oo\n");
	}else{
		printw(" oo   oo\n");
		printw("  oo oo \n");
	}
}

void poop_shape(int x, int y){
	mvprintw(y++, x, "   o   \n");
	mvprintw(y++, x, "  ooo  \n");
}

bool isCollision(const int poop_x, const int dog_y){
	if(poop_x <= 10 && poop_x >= 5){
		if(dog_y > 10)
			return true;
	}
	return false;
}

int main(void){
	initscr();
	int ch;

	while(1){
		int dog_y = DOG_Y;
		int dog_x = DOG_X;
		int poop_y = POOP_Y;
		int poop_x = POOP_X;
		int score = 0;
		bool isBottom = true;
		jump = false;
		time_t ttime = time(NULL);
	
		while(1){
			nodelay(stdscr, TRUE);
			ch = getch();
			mvprintw(0,0,"%d", ch);
			if(ch == ' ' && isBottom){
				if(poop_y == POOP_Y){
					if(dog_y <= dog_y-4){
						break;
					}
					jump = true;
					isBottom = false;
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

			if(dog_y >= 18){
				dog_y = 18;
				isBottom = true;
			}

			if(dog_y <= 14){
				jump = false;
			}

			poop_shape(poop_x, POOP_Y);
			dog_shape(dog_x, dog_y);

			mvprintw(5, 5, "hello");
			refresh();
			mvprintw(6, 5, "%d", poop_x);
			if(poop_x <= 6 && poop_x >= 5){
				mvprintw(7, 5, "hello");
				refresh();
				if(!jump){
					break;
				}
			}

			if(poop_x == dog_x){
				score++;
				ttime = time(NULL);
			}
			mvprintw(0, 25, "---------PRESENT SCORE: %d----------", score);
			refresh();
			napms(1000/15);
		}
		mvprintw(4, 5, "**********************\n");
		mvprintw(5, 5, "*********GAME*********\n");
		mvprintw(6, 5, "*********OVER*********\n");
		mvprintw(7, 5, "**********************\n");
		mvprintw(10, 5, "[FINAL SCORE: %d]", score);

		ch = getKeyWait();
		if(ch == 'q'){
			break;
		}
	}

	endwin();
	return 0;
}
