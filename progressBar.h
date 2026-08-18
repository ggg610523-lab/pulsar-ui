#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <SDL2/SDL.h>
#include "ui.h"

typedef struct {
    SDL_Rect rect;
    float value;
    float displayValue;
} UIProgressBar;

void progress_bar_init(
    UIProgressBar *bar,
    float value
);

void progress_bar_layout(
    UIProgressBar *bar,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void progress_bar_draw(
    UIProgressBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    float dt
);

void progress_bar_set_value(
    UIProgressBar *bar,
    float value
);

#endif /* PROGRESS_BAR_H */
