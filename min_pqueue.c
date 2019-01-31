#include "common.h"
#include "min_pqueue.h"

Min_pqueue *
min_pq_init(void)
{
    Min_pqueue *p;

    malloc_node(p, Min_pqueue);
    bzero(p, sizeof(*p));

    return p;
}

void
min_pq_destroy(Min_pqueue **p)
{
    int i;

    assert(p && *p);
    for (i = 1; i < MAXVAL; i++)
        free((*p)->ps[i]);
    free(*p);
    *p = NULL;
}

static void
min_heap(Min_pqueue *p, int i)
{
    int l, r, smallest;

    l = left(i);
    r = right(i);

    if (l > p->pqsize)
        return;
    if (p->ps[l]->_f < p->ps[i]->_f)
        smallest = l;
    else
        smallest = i;
    if (r > p->pqsize)
        goto next;
    if (p->ps[r]->_f < p->ps[smallest]->_f)
        smallest = r;
next:
    if (smallest != i) {
        swap(p->ps[i], p->ps[smallest]);
        min_heap(p, smallest);
    }
}

void
min_pq_push(Min_pqueue *p, Path *s)
{
    int i, j;

    assert(p);
    if ((i = ++p->pqsize) >= MAXVAL)
        error("heap overflow");

    copy_node(p->ps[i], s, Path);
    j = parent(i);
    while (i > 1 && p->ps[i]->_f < p->ps[j]->_f) {
        swap(p->ps[i], p->ps[j]);
        i = j;
        j = parent(i);
    }
}

Path *
min_pq_pop(Min_pqueue *p)
{
    Path *s;

    assert(p);
    if (p->pqsize < 1)
        return NULL;

    s = p->ps[1];
    p->ps[1] = p->ps[p->pqsize];
    p->ps[p->pqsize--] = s;
    min_heap(p, 1);
    return s;
}
