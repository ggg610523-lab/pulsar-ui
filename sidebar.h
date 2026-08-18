#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "ui.h"

#define SIDEBAR_ITEMS 4

typedef struct {
    SDL_Rect rect;
    float anim;
    bool open;
    const char *items[SIDEBAR_ITEMS];
} UISidebar;

void sidebar_init(
    UISidebar *sb
);

void sidebar_layout(
    UISidebar *sb,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void sidebar_event(
    UISidebar *sb,
    SDL_Event *event
);

void sidebar_draw(
    UISidebar *sb,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

#endif /* SIDEBAR_H */
