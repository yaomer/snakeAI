#include <curses.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "common.h"
#include "snake.h"
#include "queue.h"
#include "stack.h"
#include "hash.h"

/*
 * LINES = 33
 * COLS = 123
 */
int visited[34][124];
int dx[] = {0, 2, 0, -2};
int dy[] = {2, 0, -2, 0};

static void
back_path(Hash *H, Node *s, Node *d)
{
    _Hash *h;

    while ((h = ht_search(H, d))) {
        if (h->pres->x == s->x && h->pres->y == s->y) {
            next_step = *h->s;
            break;
        }
        d = h->pres;
    }
}

int
bfs(int x1, int y1, int x2, int y2)
{
    Queue *Q = queue_init();
    Hash *H = ht_init();
    Node *s, *d, *ts;
    int foundpath = 0;

    malloc_node(s, Node);
    malloc_node(d, Node);
    s->x = x1;
    s->y = y1;
    d->x = x2;
    d->y = y2;

    bzero(visited, sizeof(visited));
    queue_push(Q, s);
    ht_insert(H, s, NULL);
    visited[s->x][s->y] = 1;

    while (!is_queue_empty(Q)) {
        _Queue *q = queue_top(Q);
        for (int i = 0; i < 4; i++) {
            int tx = q->s->x + dx[i];
            int ty = q->s->y + dy[i];
            if (!visited[tx][ty] && isok(tx, ty)
                    && !is_crash_snake(tx, ty) ) {
                malloc_node(ts, Node);
                ts->x = tx;
                ts->y = ty;

                queue_push(Q, ts);
                ht_insert(H, ts, q->s);
                visited[tx][ty] = 1;
                free(ts);

                if (tx == d->x && ty == d->y) {
                    back_path(H, s, d);
                    foundpath = 1;
                    break;
                }
            }
        }
        queue_pop(Q);
        if (foundpath)
            break;
    }
    free(s);
    free(d);
    queue_clear(&Q);
    ht_destroy(&H);
    return foundpath;
}

int
dfs(int x1, int y1, int x2, int y2)
{
    Hash *H = ht_init();
    Stack *sp = NULL;
    Node *s, *d, *tn;
    int foundpath = 0;

    malloc_node(s, Node);
    malloc_node(d, Node);
    s->x = x1;
    s->y = y1;
    d->x = x2;
    d->y = y2;

    bzero(visited, sizeof(visited));
    ht_insert(H, s, NULL);
    stack_push(&sp, s);
    visited[s->x][s->y] = 1;

    while (!is_stack_empty(sp)) {
        Stack *ts = stack_pop(&sp);
        for (int i = 0; i < 4; i++) {
            int tx = ts->s->x + dx[i];
            int ty = ts->s->y + dy[i];
            if (!visited[tx][ty] && isok(tx, ty)
                    && !is_crash_snake(tx, ty) ) {
                malloc_node(tn, Node);
                tn->x = tx;
                tn->y = ty;

                stack_push(&sp, tn);
                ht_insert(H, tn, ts->s);
                visited[tx][ty] = 1;
                free(tn);

                if (tx == d->x && ty == d->y) {
                    back_path(H, s, d);
                    foundpath = 1;
                    break;
                }
            }
        }
        stack_free(ts);
        if (foundpath)
            break;
    }
    free(s);
    free(d);
    stack_clear(&sp);
    ht_destroy(&H);
    return foundpath;
}
