#include <curses.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "move.h"
#include "path.h"

Snake *snake;
Node food;
static int find_tail;
extern Node next_step;

static int
is_eat_food(void)
{
    return snake->head->s->x == food.x
        && snake->head->s->y == food.y;
}

int
is_crash_snake(int x, int y)
{
    struct snake_node *p = snake->tail;

    if (find_tail) {
        while (p) {
            if (p != snake->tail && x == p->s->x && y == p->s->y)
                return 1;
            p = p->next;
        }
    } else {
        while (p) {
            if (x == p->s->x && y == p->s->y)
                return 1;
            p = p->next;
        }
    }
    return 0;
}

static void
over(void)
{
    attron(COLOR_PAIR(5));
    mvaddstr(15, 40, "GAME OVER");
    attroff(COLOR_PAIR(5));
    refresh();
    sleep(5);
    endwin();
    exit(0);
}

static int
can_find_path_to_eat_food(void)
{
    Node *head = snake->head->s;
    return search_min_path(head->x, head->y, food.x, food.y);
}

static int
can_find_tail_after_eat_food(int x, int y)
{
    Node *tail = snake->tail->s;
    return search_min_path(x, y, tail->x, tail->y);
}

static int
can_find_tail(void)
{
    Node *head = snake->head->s;
    Node *tail = snake->tail->s;
    return search_min_path(head->x, head->y, tail->x, tail->y);
}

void
display_snake(int signo)
{
    if (can_find_path_to_eat_food()) {
        int tx = next_step.x;
        int ty = next_step.y;
        find_tail = 1;
        if (can_find_tail_after_eat_food(tx, ty)) {
            add_snake(tx, ty);
        } else {
            if (can_find_tail()) {
                add_snake(next_step.x, next_step.y);
            } else
                wander();
        }
    } else {
        find_tail = 1;
        if (can_find_tail()) {
            add_snake(next_step.x, next_step.y);
        } else
            wander();
    }
    find_tail = 0;

    if (is_eat_food()) {
        display_score();
        creat_food();
    } else
        del_snake();
    refresh();
}

void
display_score(void)
{
    static int score = 0;
    attron(COLOR_PAIR(5));
    if (score == 600)
        over();
    mvaddstr(11, 97, "^o^YOUR SCORE^o^");
    mvprintw(12, 104, "%d", ++score);
    attroff(COLOR_PAIR(5));
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
    struct snake_node *p, *q;

    malloc_node(p, struct snake_node);
    malloc_node(p->s, Node);
    malloc_node(q, struct snake_node);
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
    struct snake_node *tail = snake->tail;
    struct snake_node *tmp = tail->next->next;

    mvaddch(tail->s->x, tail->s->y, ' ');
    mvaddch(tail->next->s->x, tail->next->s->y, ' ');
    free(tail->next->s);
    free(tail->next);
    free(tail->s);
    free(tail);
    snake->tail = tmp;
}

void
clear_snake(void)
{
    struct snake_node *tmp;

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
    malloc_node(snake->head = snake->tail, struct snake_node);
    malloc_node(snake->head->s, Node);
    snake->head->next = NULL;
    srand(clock());
    while ((snake->head->s->x = rand() % (LINES - 6) + 3) % 2 == 0)
        ;
    while ((snake->head->s->y = rand() % (COLS - 38) + 4) % 2 != 0)
        ;
    attron(COLOR_PAIR(1));
    mvaddch(snake->head->s->x, snake->head->s->y, ' ');
    attroff(COLOR_PAIR(1));
}

void creat_food(void)
{
    srand(clock());
    while ((food.x = rand() % (LINES - 6) + 3) % 2 == 0)
        ;
    while ((food.y = rand() % (COLS - 38) + 4) % 2 != 0)
        ;
    if (is_crash_snake(food.x, food.y))   /*  食物不能覆盖蛇身  */
        creat_food();
    attron(COLOR_PAIR(2));
    mvaddch(food.x, food.y, ' ');
    attroff(COLOR_PAIR(2));
}
