#ifndef TOGGLE_H
#define TOGGLE_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;

    bool on;
    bool clicked;

    float thumb_x;
    float anim;
} UIToggle;

void toggle_init(
    UIToggle *toggle,
    bool on
);

void toggle_layout(
    UIToggle *toggle,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void toggle_event(
    UIToggle *toggle,
    SDL_Event *event
);

void toggle_draw(
    UIToggle *toggle,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* TOGGLE_H */
