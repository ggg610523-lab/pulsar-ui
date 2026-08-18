#ifndef HAMBURGER_H
#define HAMBURGER_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;
    bool open;
    bool clicked;
    float anim;
} UIHamburger;

void hamburger_init(
    UIHamburger *hb
);

void hamburger_layout(
    UIHamburger *hb,
    UIContext *ui,
    float x,
    float y,
    float size
);

void hamburger_event(
    UIHamburger *hb,
    SDL_Event *event
);

void hamburger_draw(
    UIHamburger *hb,
    UIContext *ui,
    SDL_Renderer *renderer
);

#endif /* HAMBURGER_H */
