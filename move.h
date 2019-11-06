#ifndef _MOVE_H
#define _MOVE_H

typedef struct {
    int x;
    int y;
} Node;

struct snake_node {
    Node *s;
    struct snake_node *next;
};

typedef struct {
    struct snake_node *head;
    struct snake_node *tail;
} Snake;

extern Snake *snake;

void    display_snake(int signo);

void    display_score(void);
void    creat_snake(void);
void    creat_food(void);

void    init_snake(void);
void    add_snake(int x, int y);
void    del_snake(void);
void    clear_snake(void);

int is_crash_snake(int x, int y);

#endif  /* _MOVE_H */
