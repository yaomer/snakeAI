#ifndef _SNAKE_H
#define _SNAKE_H

#define DELAY   20    /*  设置延时  */

#define isok(x, y)  ((x) >= 3 && (x) <= (LINES - 3) && (y) >= 4 && (y) <= (COLS - 5))

typedef struct {
    int x;
    int y;
} Node;

#include "move.h"

extern Snake *snake;
extern Node next_step;
extern Node food;

void    init(void);
void    swait(void);
void    draw_map(void);
void    set_color(void);
int     set_ticker(int);

#endif   /*  _SNAKE_H  */
