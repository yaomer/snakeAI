#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"
#include "snake.h"
#include "queue.h"

Queue *
queue_init(void)
{
    Queue *p;

    malloc_node(p, Queue);
    bzero(p, sizeof(Queue));

    return p;
}

void
queue_push(Queue *Q, Node *s)
{
    _Queue *p;

    malloc_node(p, _Queue);
    copy_node(p->s, s, Node);

    if (is_queue_empty(Q))
        Q->front = Q->rear = p;
    else {
        Q->rear->next = p;
        Q->rear = p;
    }
    p->next = NULL;
}

void
queue_pop(Queue *Q)
{
    _Queue *tmp;

    if (!is_queue_empty(Q)) {
        tmp = Q->front->next;
        free(Q->front->s);
        free(Q->front);
        if (!(Q->front = tmp))
            Q->rear = NULL;
    } else
        error("queue is empty");
}

_Queue *
queue_top(Queue *Q)
{
    if (!is_queue_empty(Q))
        return Q->front;
    else
        error("queue is empty");
}

int
is_queue_full(Queue *Q)
{
    return 0;
}

int
is_queue_empty(Queue *Q)
{
    assert(Q);
    return Q->front == NULL;
}

void
queue_clear(Queue **Q)
{
    assert(Q);
    while (!is_queue_empty(*Q))
        queue_pop(*Q);
    free(*Q);
    *Q = NULL;
}
