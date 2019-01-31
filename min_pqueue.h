#ifndef _MIN_PQUEUE_H
#define _MIN_PQUEUE_H

#include "snake.h"

/*
 * LINES = 33
 * COLS = 123
 */
#define MAXVAL (34 * 124 + 1)

typedef struct {
    int pqsize;
    Path *ps[MAXVAL];
} Min_pqueue; 

#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)
/*
 * 对于宏parent(), left(), right(),
 * 下标从1开始
 */

Min_pqueue  *min_pq_init(void);
void    min_pq_destroy(Min_pqueue **);
void    min_pq_push(Min_pqueue *, Path *);
Path    *min_pq_pop(Min_pqueue *);

#endif  /* _MIN_PQUEUE_H */
