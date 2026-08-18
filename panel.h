#ifndef PANEL_H
#define PANEL_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;
    const char *title;
} UIPanel;

void panel_init(
    UIPanel *panel,
    const char *title
);

void panel_layout(
    UIPanel *panel,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void panel_draw(
    UIPanel *panel,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* PANEL_H */
