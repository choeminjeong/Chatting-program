#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define DOG_Y 18
#define DOG_X 20
#define POOP_Y 22 
#define POOP_X 40

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
	printw(" oo   oo\n");
	printw("  oo oo \n");
}

void dog_jump_shape(int x, int y){	
	move(y, x);
	printw("  o   ooo\n");
	printw(" o   ooooo\n");
	printw("ooooooooo\n");
       	printw(" oooooooo\n");	
	printw(" oo   oo\n");
	printw("oo     oo\n");
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
		bool jump = false;
		time_t ttime = time(NULL);
	
		while(1){
			ch = getKeyNoWait();
			if(ch == ' '){
				if(poop_y == POOP_Y){
					if(dog_y <= dog_y-4){
						break;
					}
					jump = true;
				} 
			}else{
				jump = false;
				if(dog_y >= DOG_Y){
					dog_y = DOG_Y;
				}else{
					dog_y += 4;
				}
			}

			poop_x -= 2;
			if(poop_x <= 0){
				poop_x = POOP_X;
			}
			clear();
			
			if(jump){
				dog_jump_shape(0, dog_y-4);
			}else{
				dog_shape(0, dog_y);
			}
			poop_shape(poop_x, POOP_Y);
	
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

			
		
		
		
