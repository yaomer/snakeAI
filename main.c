#include <curses.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "common.h"
#include "move.h"
#include "pqueue.h"

#define DELAY   20    /*  设置延时  */

static void
draw_map(void)
{
    int i;

    attron(COLOR_PAIR(4));
    for (i = 3; i < COLS - 2; i += 2) {
        mvaddch(2, i, ' ');
        mvaddch(LINES - 3, i, ' ');
    }
    for (i = 3; i < LINES - 2; i += 2) {
        mvaddch(i, 3, ' ');
        mvaddch(i, COLS - 34, ' ');
        mvaddch(i, COLS - 4, ' ');
    }
    attroff(COLOR_PAIR(4));
    /* 测试地图的边界 */
    /* mvaddch(3, 4, 'A');
    mvaddch(LINES - 4, 4, 'B');
    mvaddch(3, COLS - 35, 'C');
    mvaddch(LINES - 4, COLS - 35, 'C');
    mvprintw(10, 10, "%d %d\n", LINES, COLS); */
}

static void
set_color(void)
{
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_GREEN, COLOR_BLACK);
}

/*
 * 设置间隔计时器
 */
static int
set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;   /*  设置初始间隔  */
    new_timeset.it_interval.tv_usec = n_usecs;

    new_timeset.it_value.tv_sec = n_sec;      /*  设置重复间隔  */
    new_timeset.it_value.tv_usec = n_usecs;
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void
init(void)
{
    initscr();   /*  初始化curses  */
    start_color();  /*  初始化颜色表  */
    set_color();    /*  设置颜色  */
    box(stdscr, ACS_VLINE, ACS_HLINE);   /*  绘制一个同物理终端大小相同的窗口  */
    noecho();    /*  关闭键入字符的回显  */
    cbreak();    /*  字符一键入，直接传递给程序  */
    curs_set(0);    /*  隐藏光标  */
    keypad(stdscr, true);    /*  开启逻辑键  */
    draw_map();
    init_snake();
    creat_snake();
    creat_food();
    refresh();
}

static void
swait(void)
{
    int c;

    while ((c = getch()) != 'q')
        set_ticker(DELAY);
}

int
main(void)
{
    init();
    signal(SIGALRM, display_snake);
    swait();
    clear_snake();
    endwin();
    return 0;
}
