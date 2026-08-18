#ifndef TABS_H
#define TABS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

#define TABS_MAX 8

typedef struct {
    SDL_Rect rect;
    SDL_Rect tabRects[TABS_MAX];
    const char *labels[TABS_MAX];
    int count;
    int selected;
    float indicatorX;
    float indicatorW;
} UITabs;

void tabs_init(
    UITabs *tabs
);

void tabs_add(
    UITabs *tabs,
    const char *label
);

void tabs_layout(
    UITabs *tabs,
    UIContext *ui,
    float x,
    float y,
    float w
);

void tabs_event(
    UITabs *tabs,
    SDL_Event *event
);

void tabs_draw(
    UITabs *tabs,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

#endif /* TABS_H */
