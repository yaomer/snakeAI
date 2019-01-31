#include <math.h>
#include "common.h"
#include "snake.h"
#include "hash.h"

Hash *
ht_init(void)
{
    Hash *ht;

    malloc_node(ht, Hash);
    bzero(ht, sizeof(Hash));

    return ht;
}

void
ht_destroy(Hash **ht)
{
    _Hash *np, *tmp;

    assert(ht && *ht);
    for (int i = 0; i < HASHSIZE; i++) {
        if ((np = (*ht)->hashtab[i]))
            while (np) {
                tmp = np->next;
                free(np->s);
                free(np->pres);
                free(np);
                np = tmp;
            }
    }
    free(*ht);
    *ht = NULL;
}

static int
num_bit(int x)
{
    int i = 0;

    do {
        x /= 10;
        i++;
    } while (x);

    return i;
}

static unsigned
hash(Path *s)
{
    unsigned hashval;

    assert(s);
    hashval = s->x * pow(10, num_bit(s->y)) + s->y;
    return hashval % HASHSIZE;
}

_Hash *
ht_search(Hash *ht, Path *s)
{
    _Hash *np;

    assert(ht && s);
    for (np = ht->hashtab[hash(s)]; np; np = np->next)
        if (np->s->x == s->x && np->s->y == s->y)
            break;
    return np;
}

void
ht_insert(Hash *ht, Path *s, Path *pres)
{
    _Hash *np;
    unsigned hashval;

    assert(ht && s);
    hashval = hash(s);
    malloc_node(np, _Hash);
    copy_node(np->s, s, Path);
    copy_node(np->pres, pres, Path);
    np->next = ht->hashtab[hashval];
    ht->hashtab[hashval] = np;
}
