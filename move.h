#ifndef _MOVE_H
#define _MOVE_H

#include "snake.h"

typedef struct snake {
    Node *s;
    struct snake *next;
} _Snake;

typedef struct {
    _Snake *head;
    _Snake *tail;
} Snake;

void    display_snake(int signo);
int     bfs(int x1, int y1, int x2, int y2);
int     dfs(int x1, int y1, int x2, int y2);
void    around(void);

void    creat_snake(void);
void    creat_food(void);

void    init_snake(void);
void    add_snake(int x, int y);
void    del_snake(void);
void    clear_snake(void);

int     is_eat_food(void);
int     is_crash_snake(int x, int y);

#endif  /* _MOVE_H */
