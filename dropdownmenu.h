#ifndef DROPDOWN_MENU_H
#define DROPDOWN_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

#define DROPDOWN_MAX_ITEMS 16

typedef struct {
    SDL_Rect rect;
    SDL_Rect listRect;
    const char *items[DROPDOWN_MAX_ITEMS];
    int itemCount;
    int selected;
    bool open;
    bool hovered;
    float anim;
} UIDropDown;

void dropdown_init(
    UIDropDown *dd
);

void dropdown_add_item(
    UIDropDown *dd,
    const char *item
);

void dropdown_layout(
    UIDropDown *dd,
    UIContext *ui,
    float x,
    float y,
    float w
);

void dropdown_event(
    UIDropDown *dd,
    SDL_Event *event
);

void dropdown_draw(
    UIDropDown *dd,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

#endif /* DROPDOWN_MENU_H */
