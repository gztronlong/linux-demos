#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "circle.h"

static void circle_destroy(Shape *shape);
static void circle_draw(Shape *shape);

Circle *circle_new(int x, int y, int r) {
    Circle *circle = (Circle *)malloc(sizeof(Circle));

    shape_init((Shape *)circle, x, y);
    circle->base.destroy = circle_destroy;
    circle->base.draw = circle_draw;

    circle->r = r;

    return circle;
}

static void circle_destroy(Shape *shape) {
    assert(shape);

    Circle *this = (Circle *)shape;
    free(this);
}

static void circle_draw(Shape *shape) {
    assert(shape);

    Circle *this = (Circle *)shape;
    printf("circle: (X-%d)^2 + (Y-%d)^2 = %d\n",
            shape_x((Shape *)this), shape_y((Shape *)this), this->r);
}

int circle_radius(Circle *circle) {
    assert(circle);
    return circle->r;
}
