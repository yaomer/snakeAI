#include <curses.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "snake.h"
#include "move.h"

Snake *snake;
Node next_step;
Node food;
int findtail;
extern int dx[], dy[];

void
display_snake(int signo)
{
    Node *head = snake->head->s;
    Node *tail = snake->tail->s;

    if (bfs(head->x, head->y, food.x, food.y)) {   /* 能吃到食物 */
        int tx = next_step.x;
        int ty = next_step.y;
        findtail = 1;
        if (bfs(next_step.x, next_step.y, tail->x, tail->y))  /* 吃完后能找到自己的尾巴 */
            add_snake(tx, ty);  /* 去吃 */
        else {
            if (dfs(head->x, head->y, tail->x, tail->y))
                add_snake(next_step.x, next_step.y);  /* 追着自己的尾巴走一步 */
            else
                around();  /* 随便逛逛 */
        }
    } else {
        findtail = 1;
        if (dfs(head->x, head->y, tail->x, tail->y))
            add_snake(next_step.x, next_step.y);
        else
            around();
    }
    findtail = 0;

    if (is_eat_food())
        creat_food();
    else
        del_snake();
    refresh();
}

void
around(void)
{
    int max = 0, j = -1;
    Node *head = snake->head->s;
    Node *tail = snake->tail->s;

    findtail = 0;
    for (int i = 0; i < 4; i++) {
        int tx = head->x + dx[i];
        int ty = head->y + dy[i];
        if (isok(tx, ty) && !is_crash_snake(tx, ty)
                && dfs(tx, ty, tail->x, tail->y)) {
            if (max < abs(food.x - tx) + abs(food.y - ty)) {
                max = abs(food.x - tx) + abs(food.y - ty);
                j = i;
            }
        }
    }
    if (j == -1)
        for (int i = 0; i < 4; i++) {
            int tx = head->x + dx[i];
            int ty = head->y + dy[i];
            if (isok(tx, ty) && !is_crash_snake(tx, ty)) {
                if (max < abs(food.x - tx) + abs(food.y - ty)) {
                    max = abs(food.x - tx) + abs(food.y - ty);
                    j = i;
                }
            }
        }
    add_snake(head->x + dx[j], head->y + dy[j]);
}

int
is_eat_food(void)
{
    return snake->head->s->x == food.x
        && snake->head->s->y == food.y;
}

int
is_crash_snake(int x, int y)
{
    _Snake *p = snake->tail;

    if (findtail)
        while (p) {
            if (p != snake->tail && x == p->s->x && y == p->s->y)
                return 1;
            p = p->next;
        }
    else
        while (p) {
            if (x == p->s->x && y == p->s->y)
                return 1;
            p = p->next;
        }
    return 0;
}

void
init_snake(void)
{
    malloc_node(snake, Snake);
    bzero(snake, sizeof(Snake));
}

void
add_snake(int x, int y)
{
    _Snake *p, *q;

    malloc_node(p, _Snake);
    malloc_node(p->s, Node);
    malloc_node(q, _Snake);
    malloc_node(q->s, Node);

    snake->head->next = p;
    p->next = q;
    q->next = NULL;
    attron(COLOR_PAIR(1));
    p->s->x = (snake->head->s->x + x) / 2;
    p->s->y = (snake->head->s->y + y) / 2;
    mvaddch(p->s->x, p->s->y, ' ');
    q->s->x = x;
    q->s->y = y;
    mvaddch(q->s->x, q->s->y, ' ');
    attroff(COLOR_PAIR(1));
    snake->head = q;
}

void
del_snake(void)
{
    _Snake *tmp;

    mvaddch(snake->tail->s->x, snake->tail->s->y, ' ');
    mvaddch(snake->tail->next->s->x, snake->tail->next->s->y, ' ');
    tmp = snake->tail->next->next;
    free(snake->tail->next->s);
    free(snake->tail->next);
    free(snake->tail->s);
    free(snake->tail);
    snake->tail = tmp;
}

void
clear_snake(void)
{
    _Snake *tmp;

    while (snake->tail) {
        tmp = snake->tail->next;
        free(snake->tail->s);
        free(snake->tail);
        snake->tail = tmp;
    }
}

void
creat_snake(void)
{
    malloc_node(snake->head = snake->tail, _Snake);
    malloc_node(snake->head->s, Node);
    snake->head->next = NULL;
    srand(clock());
    while ((snake->head->s->x = rand() % (LINES - 5) + 3) % 2 == 0)
        ;
    while ((snake->head->s->y = rand() % (COLS - 8) + 4) % 2 != 0)
        ;
    attron(COLOR_PAIR(1));
    mvaddch(snake->head->s->x, snake->head->s->y, ' ');
    attroff(COLOR_PAIR(1));
}

void creat_food(void)
{
    srand(clock());
    while ((food.x = rand() % (LINES - 5) + 3) % 2 == 0)
        ;
    while ((food.y = rand() % (COLS - 8) + 4) % 2 != 0)
        ;
    if (is_crash_snake(food.x, food.y))   /*  食物不能覆盖蛇身  */
        creat_food();
    attron(COLOR_PAIR(2));
    mvaddch(food.x, food.y, ' ');
    attroff(COLOR_PAIR(2));
}
