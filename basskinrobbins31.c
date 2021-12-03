#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
int main(){
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
