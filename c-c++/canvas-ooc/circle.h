#ifndef _OOC_CIRCLE_H_
#define _OOC_CIRCLE_H_

#include "shape.h"

struct _Circle {
    Shape base;
    int r;
};

typedef struct _Circle Circle;

Circle *circle_new(int x, int y, int r);

int    circle_radius(Circle *circle);

#endif
