#ifndef _PQUEUE_H
#define _PQUEUE_H

#include "path.h"

#define MAXVAL (30 * 99 + 1)

typedef struct {
    int pqsize;
    Path *ps[MAXVAL];
} pqueue_t;

typedef struct {
    pqueue_t *(*init)(void);
    void (*clean)(pqueue_t *);
    void (*dealloc)(pqueue_t *);
    void (*push)(pqueue_t *, Path *);
    Path *(*pop)(pqueue_t *);
} pqueue_op_t;

extern pqueue_op_t max_pqueue_op;
extern pqueue_op_t min_pqueue_op;

#endif
