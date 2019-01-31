#include "common.h"
#include "max_pqueue.h"

Max_pqueue *
max_pq_init(void)
{
    Max_pqueue *p;

    malloc_node(p, Max_pqueue);
    bzero(p, sizeof(*p));

    return p;
}

void
max_pq_destroy(Max_pqueue **p)
{
    int i;

    assert(p && *p);
    for (i = 1; i < MAXVAL; i++)
        free((*p)->ps[i]);
    free(*p);
    *p = NULL;
}

static void
max_heap(Max_pqueue *p, int i)
{
    int l, r, longest;

    l = left(i);
    r = right(i);

    if (l > p->pqsize)
        return;
    if (p->ps[l]->_f > p->ps[i]->_f)
        longest = l;
    else
        longest = i;
    if (r > p->pqsize)
        goto next;
    if (p->ps[r]->_f > p->ps[longest]->_f)
        longest = r;
next:
    if (longest != i) {
        swap(p->ps[i], p->ps[longest]);
        max_heap(p, longest);
    }
}

void
max_pq_push(Max_pqueue *p, Path *s)
{
    int i, j;

    assert(p);
    if ((i = ++p->pqsize) >= MAXVAL)
        error("heap overflow");

    copy_node(p->ps[i], s, Path);
    j = parent(i);
    while (i > 1 && p->ps[i]->_f > p->ps[j]->_f) {
        swap(p->ps[i], p->ps[j]);
        i = j;
        j = parent(i);
    }
}

Path *
max_pq_pop(Max_pqueue *p)
{
    Path *s;

    assert(p);
    if (p->pqsize < 1)
        return NULL;

    s = p->ps[1];
    p->ps[1] = p->ps[p->pqsize];
    p->ps[p->pqsize--] = s;
    max_heap(p, 1);
    return s;
}
/*
int
main(void)
{
    Path *s[4];
    Max_pqueue *p = max_pq_init();

    for (int i = 0; i < 4; i++) {
        malloc_node(s[i], Path);
    }

    s[0]->_f = 1;
    s[1]->_f = 5;
    s[2]->_f = 8;
    s[3]->_f = 6;

    max_pq_push(p, s[0]);
    max_pq_push(p, s[1]);
    max_pq_push(p, s[2]);
    max_pq_push(p, s[3]);

    Path *t = max_pq_pop(p);
    printf("%d\n", t->_f);
    t = max_pq_pop(p);
    printf("%d\n", t->_f);
    t = max_pq_pop(p);
    printf("%d\n", t->_f);
    t = max_pq_pop(p);
    printf("%d\n", t->_f);
} */
