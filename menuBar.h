#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

#define MENUBAR_MAX_MENUS 8
#define MENUBAR_MAX_ITEMS 16

typedef struct {
    const char *label;
} MenuItem;

typedef struct {
    const char *label;
    MenuItem items[MENUBAR_MAX_ITEMS];
    int itemCount;
    SDL_Rect rect;
    SDL_Rect menuRect;
} Menu;

typedef struct {
    SDL_Rect rect;
    Menu menus[MENUBAR_MAX_MENUS];
    int menuCount;
    int openMenu;
    float anim;
} UIMenuBar;

void menubar_init(
    UIMenuBar *bar
);

void menubar_add_menu(
    UIMenuBar *bar,
    const char *label
);

void menubar_add_item(
    UIMenuBar *bar,
    int menuIndex,
    const char *item
);

void menubar_layout(
    UIMenuBar *bar,
    UIContext *ui,
    float x,
    float y,
    float w
);

void menubar_event(
    UIMenuBar *bar,
    SDL_Event *event
);

int menubar_selected(
    UIMenuBar *bar,
    int *menuIdx,
    int *itemIdx
);

void menubar_draw(
    UIMenuBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

#endif /* MENU_BAR_H */
