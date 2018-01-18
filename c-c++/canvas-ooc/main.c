#include "shape.h"
#include "canvas.h"
#include "circle.h"

int main(int argc, char **argv) {
    Canvas *canvas = canvas_new(800, 480, 0xffffff);

    Circle *circle10 = circle_new(10, 10, 10);
    canvas_add(canvas, (Shape *)circle10);

    Circle *circle20 = circle_new(20, 20, 20);
    canvas_add(canvas, (Shape *)circle20);

    /*
     * canvas_add(canvas, line);
     * canvas_add(canvas, rect);
     * ...
     */

    canvas_update(canvas);

    canvas_destroy(canvas);

    shape_destroy((Shape *)circle10);
    shape_destroy((Shape *)circle20);

    return 0;
}
