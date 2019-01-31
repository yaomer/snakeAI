#ifndef _MAX_PQUEUE_H
#define _MAX_PQUEUE_H

#include "snake.h"

#define MAXVAL (34 * 124 + 1)

typedef struct {
    int pqsize;
    Path *ps[MAXVAL];
} Max_pqueue; 

#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)
/*
 * 对于宏parent(), left(), right(),
 * 下标从1开始
 */

Max_pqueue  *max_pq_init(void);
void    max_pq_destroy(Max_pqueue **);
void    max_pq_push(Max_pqueue *, Path *);
Path    *max_pq_pop(Max_pqueue *);

#endif  /* _MAX_PQUEUE_H */
