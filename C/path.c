#include <curses.h>

#include "common.h"
#include "hash.h"
#include "pqueue.h"
#include "move.h"

/*
 * LINES - 4 = 29
 * COLS - 35 = 98
 */
static int open[30][99];
static int close[30][99];
static int dx[] = {0, 2, 0, -2};
static int dy[] = {2, 0, -2, 0};

Node next_step;
extern Node food;

/*
 * 回溯路径，找到下一步要走的坐标(x, y)
 */
static void
back_path(Hash *H, Path *s, Path *d)
{
    struct hash_node *h;

    while ((h = ht_search(H, d))) {
        if (h->pres->x == s->x && h->pres->y == s->y) {
            next_step.x = h->s->x;
            next_step.y = h->s->y;
            break;
        }
        d = h->pres;
    }
}

static Path *
init_node(int x, int y)
{
    Path *s;

    malloc_node(s, Path);
    s->x = x;
    s->y = y;
    s->_f = s->_g = s->_h = 0;

    return s;
}

#define isok(x, y)  ((x) >= 3 && (x) <= (LINES - 4) && (y) >= 4 && (y) <= (COLS - 35))

static int AStar(int x1, int y1, int x2, int y2, pqueue_op_t *op)
{
    pqueue_t *openlist = op->init();
    Hash *parent = ht_init();
    Path *s = init_node(x1, y1);
    Path *d = init_node(x2, y2);
    Path *np, *ts;
    int foundpath = 0;

    bzero(open, sizeof(open));
    bzero(close, sizeof(close));
    op->push(openlist, s);
    open[s->x][s->y] = 1;
    while ((np = op->pop(openlist))) {
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
                    op->push(openlist, ts);
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
    ht_destroy(&parent);
    op->dealloc(openlist);
    return foundpath;

}

int
search_max_path(int x1, int y1, int x2, int y2)
{
    return AStar(x1, y1, x2, y2, &max_pqueue_op);
}

int
search_min_path(int x1, int y1, int x2, int y2)
{
    return AStar(x1, y1, x2, y2, &min_pqueue_op);
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
