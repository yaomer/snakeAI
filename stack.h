#ifndef _STACK_H
#define _STACK_H

#include "snake.h"

typedef struct stack {
    Node *s;
    struct stack *next;
} Stack;

void    stack_push(Stack **, Node *);
void    stack_free(Stack *);
Stack   *stack_pop(Stack **);
void    stack_clear(Stack **);
int     is_stack_empty(Stack *);

#endif  /* _STACK_H */
