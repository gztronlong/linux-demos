#ifndef _OOC_CANVAS_H_
#define _OOC_CANVAS_H_

#include <stdbool.h>

#include "shape.h"

typedef struct _Canvas Canvas;

Canvas *canvas_new(int w, int h, int bg);
void canvas_destroy(Canvas *canvas);

bool canvas_add(Canvas *canvas, Shape *shape);

void canvas_clear_bg(Canvas *canvas);
void canvas_update(Canvas *canvas);

int canvas_width(Canvas *canvas);
int canvas_height(Canvas *canvas);

#endif
