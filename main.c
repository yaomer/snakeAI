#include <curses.h>
#include <signal.h>
#include "common.h"
#include "snake.h"
#include "move.h"

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

void
swait(void)
{
    int c;

    while ((c = getch()) != 'q')
        set_ticker(DELAY);
}

void
draw_map(void)
{
    int i;

    attron(COLOR_PAIR(4));
    for (i = 3; i < COLS - 2; i += 2) {
        mvaddch(2, i, ' ');
        mvaddch(LINES - 2, i, ' ');
    }
    for (i = 3; i < LINES - 1; i += 2) {
        mvaddch(i, 3, ' ');
        mvaddch(i, COLS - 4, ' ');
    }
    attroff(COLOR_PAIR(4));
    /* 测试地图的边界 */
    /* mvaddch(3, 4, 'A');
    mvaddch(LINES - 3, 4, 'B');
    mvaddch(3, COLS - 5, 'C');
    mvaddch(LINES - 3, COLS - 5, 'C');
    mvprintw(10, 10, "%d %d\n", LINES, COLS); */
}
