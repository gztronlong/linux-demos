#ifndef _OOC_SHAPE_H_
#define _OOC_SHAPE_H_

typedef struct _Shape Shape;
struct _Shape {
    int x;
    int y;

    void (* destroy)(Shape *shape);
    void (* draw)(Shape *shape);
};

void shape_init(Shape *shape, int x, int y);
void shape_destroy(Shape *shape);

void shape_draw(Shape *shape);

int  shape_x(Shape *shape);
int  shape_y(Shape *shape);

#endif
