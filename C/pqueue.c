#include <stdio.h>

#include "common.h"
#include "pqueue.h"

/* 下标从1开始 */
#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)

static pqueue_t *
pqueue_init(void)
{
    pqueue_t *pq;
    malloc_node(pq, pqueue_t);
    bzero(pq, sizeof(pqueue_t));
    return pq;
}

static void
pqueue_clear(pqueue_t *pq)
{
    for (int i = 0; i < pq->pqsize; i++)
        free(pq->ps[i]);
}

static void
pqueue_dealloc(pqueue_t *pq)
{
    pqueue_clear(pq);
    free(pq);
}

static void
max_pqueue_push(pqueue_t *pq, Path *s)
{
    int i, j;

    if ((i = ++pq->pqsize) >= MAXVAL) {
        fprintf(stderr, "heap overflow\n");
        exit(1);
    }

    copy_node(pq->ps[i], s, Path);
    j = parent(i);
    while (i > 1 && pq->ps[i]->_f > pq->ps[j]->_f) {
        swap(pq->ps[i], pq->ps[j]);
        i = j;
        j = parent(i);
    }
}

static void
min_pqueue_push(pqueue_t *pq, Path *s)
{
    int i, j;

    if ((i = ++pq->pqsize) >= MAXVAL) {
        fprintf(stderr, "heap overflow\n");
        exit(1);
    }

    copy_node(pq->ps[i], s, Path);
    j = parent(i);
    while (i > 1 && pq->ps[i]->_f < pq->ps[j]->_f) {
        swap(pq->ps[i], pq->ps[j]);
        i = j;
        j = parent(i);
    }
}

static void
max_heap(pqueue_t *pq, int i)
{
    int l, r, longest;

    l = left(i);
    r = right(i);

    if (l > pq->pqsize)
        return;
    if (pq->ps[l]->_f > pq->ps[i]->_f)
        longest = l;
    else
        longest = i;
    if (r > pq->pqsize)
        goto next;
    if (pq->ps[r]->_f > pq->ps[longest]->_f)
        longest = r;
next:
    if (longest != i) {
        swap(pq->ps[i], pq->ps[longest]);
        max_heap(pq, longest);
    }
}

static Path *
max_pqueue_pop(pqueue_t *pq)
{
    Path *s;

    if (pq->pqsize < 1)
        return NULL;

    s = pq->ps[1];
    pq->ps[1] = pq->ps[pq->pqsize];
    pq->ps[pq->pqsize--] = s;
    max_heap(pq, 1);
    return s;
}

static void
min_heap(pqueue_t *pq, int i)
{
    int l, r, smallest;

    l = left(i);
    r = right(i);

    if (l > pq->pqsize)
        return;
    if (pq->ps[l]->_f < pq->ps[i]->_f)
        smallest = l;
    else
        smallest = i;
    if (r > pq->pqsize)
        goto next;
    if (pq->ps[r]->_f < pq->ps[smallest]->_f)
        smallest = r;
next:
    if (smallest != i) {
        swap(pq->ps[i], pq->ps[smallest]);
        min_heap(pq, smallest);
    }
}

static Path *
min_pqueue_pop(pqueue_t *pq)
{
    Path *s;

    if (pq->pqsize < 1)
        return NULL;

    s = pq->ps[1];
    pq->ps[1] = pq->ps[pq->pqsize];
    pq->ps[pq->pqsize--] = s;
    min_heap(pq, 1);
    return s;
}

pqueue_op_t max_pqueue_op = {
    pqueue_init,
    pqueue_clear,
    pqueue_dealloc,
    max_pqueue_push,
    max_pqueue_pop,
};

pqueue_op_t min_pqueue_op = {
    pqueue_init,
    pqueue_clear,
    pqueue_dealloc,
    min_pqueue_push,
    min_pqueue_pop,
};
