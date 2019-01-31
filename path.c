#include <curses.h>
#include "common.h"
#include "max_pqueue.h"
#include "min_pqueue.h"
#include "hash.h"
#include "snake.h"

/*
 * LINES - 4 = 29
 * COLS - 35 = 98
 */
int open[30][99];
int close[30][99];
int dx[] = {0, 2, 0, -2};
int dy[] = {2, 0, -2, 0};

Node next_step;

void
back_path(Hash *H, Path *s, Path *d)
{
    _Hash *h;

    while ((h = ht_search(H, d))) {
        if (h->pres->x == s->x && h->pres->y == s->y) {
            next_step.x = h->s->x;
            next_step.y = h->s->y;
            break;
        }
        d = h->pres;
    }
}

Path *
init_node(int x, int y)
{
    Path *s;

    malloc_node(s, Path);
    s->x = x;
    s->y = y;
    s->_f = s->_g = s->_h = 0;

    return s;
}

int
search_max_path(int x1, int y1, int x2, int y2)
{
    Max_pqueue *openlist = max_pq_init();
    Hash *parent = ht_init();
    Path *s = init_node(x1, y1);
    Path *d = init_node(x2, y2);
    Path *np, *ts;
    int foundpath = 0;

    bzero(open, sizeof(open));
    bzero(close, sizeof(close));
    max_pq_push(openlist, s);
    open[s->x][s->y] = 1;
    while ((np = max_pq_pop(openlist))) {
        close[np->x][np->y] = 1;
        for (int i = 0; i < 4; i++) {
            int tx = np->x + dx[i];
            int ty = np->y + dy[i];
            if (!close[tx][ty] && isok(tx, ty)
                    && !is_crash_snake(tx, ty)) {
                if (!open[tx][ty]) {
                    ts = init_node(tx, ty);
                    ts->_g = np->_g + 1;
                    ts->_h = abs(tx - d->x) + abs(ty - d->y);
                    ts->_f = ts->_g + ts->_h;
                    ht_insert(parent, ts, np);
                    max_pq_push(openlist, ts);
                    open[tx][ty] = 1;
                    free(ts);
                }
            }
            if (np->x == d->x && np->y == d->y) {
                back_path(parent, s, d);
                foundpath = 1;
                break;
            }
        }
        if (foundpath)
            break;
    }
    free(s);
    free(d);
    max_pq_destroy(&openlist);
    ht_destroy(&parent);
    return foundpath;
}

int
search_min_path(int x1, int y1, int x2, int y2)
{
    Min_pqueue *openlist = min_pq_init();
    Hash *parent = ht_init();
    Path *s = init_node(x1, y1);
    Path *d = init_node(x2, y2);
    Path *np, *ts;
    int foundpath = 0;

    bzero(open, sizeof(open));
    bzero(close, sizeof(close));
    min_pq_push(openlist, s);
    open[s->x][s->y] = 1;
    while ((np = min_pq_pop(openlist))) {
        close[np->x][np->y] = 1;
        for (int i = 0; i < 4; i++) {
            int tx = np->x + dx[i];
            int ty = np->y + dy[i];
            if (!close[tx][ty] && isok(tx, ty)
                    && !is_crash_snake(tx, ty)) {
                if (!open[tx][ty]) {
                    ts = init_node(tx, ty);
                    ts->_g = np->_g + 1;
                    ts->_h = abs(tx - d->x) + abs(ty - d->y);
                    ts->_f = ts->_g + ts->_h;
                    ht_insert(parent, ts, np);
                    min_pq_push(openlist, ts);
                    open[tx][ty] = 1;
                    free(ts);
                }
            }
            if (np->x == d->x && np->y == d->y) {
                back_path(parent, s, d);
                foundpath = 1;
                break;
            }
        }
        if (foundpath)
            break;
    }
    free(s);
    free(d);
    min_pq_destroy(&openlist);
    ht_destroy(&parent);
    return foundpath;
}

void
wander(void)
{
    Node *head = snake->head->s;
    Node *tail = snake->tail->s;
    int shortest = 0, j = -1;

    for (int i = 0; i < 4; i++) {
        int tx = head->x + dx[i];
        int ty = head->y + dy[i];
        if (isok(tx, ty) && !is_crash_snake(tx, ty)
                && search_max_path(tx, ty, tail->x, tail->y)) {
            if (shortest == 0) {
                shortest = abs(tx - food.x) + abs(ty - food.y);
                j = i;
            } else if (shortest > abs(tx - food.x) + abs(ty - food.y)) {
                shortest = abs(tx - food.x) + abs(ty - food.y);
                j = i;
            }
        }
    }
    add_snake(head->x + dx[j], head->y + dy[j]);
}
