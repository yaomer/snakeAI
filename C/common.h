#ifndef _COMMON_H
#define _COMMON_H

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define malloc_node(p, type) (assert(p = malloc(sizeof(type))))

#define copy_node(p1, p2, type) do {  \
    if (p2) {  \
        malloc_node(p1, type);  \
        memcpy(p1, p2, sizeof(type));  \
    } else  \
        p1 = p2;  \
} while (0)

#define swap(a, b) do {  \
    typeof(a) tmp = a;  \
    a = b;  \
    b = tmp;  \
} while (0)

#endif  /* _COMMON_H */
