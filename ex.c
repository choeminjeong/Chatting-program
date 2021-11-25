#include <ncurses.h>
int main()
{
    WINDOW *win, *win2, *win3;
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    refresh();
    win = newwin(15, 70, 0, 1);
	win2 = newwin(25, 30, 0, 71);
	win3 = newwin(10, 70, 15, 1);

    wbkgd(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(1));
    wbkgd(win2, COLOR_PAIR(1));
    wattron(win2, COLOR_PAIR(1));
    wbkgd(win3, COLOR_PAIR(1));
    wattron(win3, COLOR_PAIR(1));

    mvwprintw(win, 1, 1, "chatting box");
    wborder(win, ' ',' ',' ',' ',' ',' ',' ',' ');
    box(win, 0, 0);
    wrefresh(win);

    mvwprintw(win2, 1, 1, "Logged Users:");
    wborder(win2, ' ',' ',' ',' ',' ',' ',' ',' ');
    box(win2, 0, 0);
    wrefresh(win2);

    mvwprintw(win3, 1, 1, "enter the message");
    wborder(win, ' ',' ',' ',' ',' ',' ',' ',' ');
    box(win3, 0, 0);
	wrefresh(win3);

    getch();

    delwin(win);
    endwin();
}
