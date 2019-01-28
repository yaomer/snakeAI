#ifndef _QUEUE_H
#define _QUEUE_H

#include "snake.h"

typedef struct queue {
    Node *s;
    struct queue *next;
} _Queue;

typedef struct {
    _Queue *front;
    _Queue *rear;
} Queue;

Queue   *queue_init(void);
void    queue_push(Queue *, Node *);
void    queue_pop(Queue *);
_Queue  *queue_top(Queue *);
void    queue_clear(Queue **);
int     is_queue_full(Queue *);
int     is_queue_empty(Queue *);

#endif  /* _QUEUE_H */
