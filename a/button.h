#ifndef BUTTON_H
#define BUTTON_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;

    const char *text;

    bool hovered;
    bool pressed;
    bool clicked;
} UIButton;

void button_init(
    UIButton *button,
    const char *text
);

void button_layout(
    UIButton *button,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void button_event(
    UIButton *button,
    SDL_Event *event
);

void button_draw(
    UIButton *button,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float time
);

#endif /* BUTTON_H */
