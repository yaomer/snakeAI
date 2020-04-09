#ifndef _PATH_H
#define _PATH_H

typedef struct {
    int x;
    int y;
    int _f;
    int _g;
    int _h;
} Path;

int search_min_path(int x1, int y1, int x2, int y2);
int search_max_path(int x1, int y1, int x2, int y2);
void wander(void);

#endif
