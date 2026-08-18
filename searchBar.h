#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include "ui.h"

#define SEARCHBAR_MAX 256

typedef struct {
    SDL_Rect rect;

    char text[SEARCHBAR_MAX];
    int len;

    bool focused;
    float cursor_blink;
} UISearchBar;

void searchbar_init(
    UISearchBar *bar
);

void searchbar_layout(
    UISearchBar *bar,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void searchbar_event(
    UISearchBar *bar,
    SDL_Event *event
);

void searchbar_draw(
    UISearchBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

#endif /* SEARCHBAR_H */
