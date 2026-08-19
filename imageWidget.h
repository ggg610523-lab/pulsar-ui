#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "image.h"
#include "ui.h"

typedef struct {
    Image image;
    Gif gif;
    bool is_gif;
    bool loaded;
    SDL_Rect rect;
} ImageWidget;

void image_widget_init(
    ImageWidget *iw,
    SDL_Renderer *renderer,
    const char *path
);

void image_widget_free(
    ImageWidget *iw
);

void image_widget_update(
    ImageWidget *iw,
    float dt
);

void image_widget_draw(
    ImageWidget *iw,
    SDL_Renderer *renderer,
    int x,
    int y,
    int w,
    int h
);

#endif /* IMAGEWIDGET_H */
