#ifndef _HASH_H
#define _HASH_H

#include "path.h"

#define HASHSIZE 3456

struct hash_node {
    Path *s;
    Path *pres;
    struct hash_node *next;
};

typedef struct {
    struct hash_node *hashtab[HASHSIZE];
} Hash;

Hash    *ht_init(void);
struct hash_node   *ht_search(Hash *, Path *);
void    ht_insert(Hash *, Path *, Path *);
void    ht_destroy(Hash **);

#endif  /* _HASH_H */
