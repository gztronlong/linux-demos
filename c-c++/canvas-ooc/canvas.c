#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "canvas.h"

struct _Canvas {
    int w;
    int h;
    int bg;

    Shape *shapes[1024];
    int count;
};

Canvas *canvas_new(int w, int h, int bg) {
    Canvas *canvas = (Canvas *)malloc(sizeof(Canvas));
    memset(canvas, 0, sizeof(Canvas));

    canvas->w = w;
    canvas->h = h;
    canvas->bg = bg;
    canvas->count = 0;

    return canvas;
}

void canvas_destroy(Canvas *canvas) {
    assert(canvas);
    free(canvas);
}

bool canvas_add(Canvas *canvas, Shape *shape) {
    assert(canvas);
    assert(shape);

    if (canvas->count == 1024) {
        printf("warning: the maximum count of shape in canvas is 1024\n");
        return false;
    }

    canvas->shapes[canvas->count] = shape;
    canvas->count++;

    return true;
}

void canvas_clear_bg(Canvas *canvas) {
    assert(canvas);
    printf("canvas: set background color as %x\n", canvas->bg);
}

void canvas_update(Canvas *canvas) {
    assert(canvas);

    canvas_clear_bg(canvas);

    int i = 0;
    for (i = 0; i < canvas->count; i++)
        shape_draw(canvas->shapes[i]);
}

int canvas_width(Canvas *canvas) {
    assert(canvas);
    return canvas->w;
}

int canvas_height(Canvas *canvas) {
    assert(canvas);
    return canvas->h;
}
