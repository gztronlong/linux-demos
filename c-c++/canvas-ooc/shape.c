#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "shape.h"

void shape_init(Shape *shape, int x, int y) {
    assert(shape);

    shape->x = x;
    shape->y = y;
}

void shape_destroy(Shape *shape) {
    assert(shape);
    shape->destroy(shape);
}

void shape_draw(Shape *shape) {
    assert(shape);
    shape->draw(shape);
}

int  shape_x(Shape *shape) {
    assert(shape);
    return shape->x;
}

int  shape_y(Shape *shape) {
    assert(shape);
    return shape->y;
}
