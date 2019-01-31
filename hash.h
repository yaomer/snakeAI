#ifndef _HASH_H
#define _HASH_H

#include "snake.h"

#define HASHSIZE 12345

typedef struct hash {
    Path *s;
    Path *pres;
    struct hash *next;
} _Hash;

typedef struct {
    _Hash *hashtab[HASHSIZE];
} Hash;

Hash    *ht_init(void);
_Hash   *ht_search(Hash *, Path *);
void    ht_insert(Hash *, Path *, Path *);
void    ht_destroy(Hash **);

#endif  /* _HASH_H */
