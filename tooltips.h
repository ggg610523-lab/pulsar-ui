#ifndef TOOLTIPS_H
#define TOOLTIPS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

typedef struct {
    SDL_Rect triggerRect;
    const char *text;
    bool visible;
    float alpha;
    float timer;
} UITooltip;

typedef struct {
    UITooltip tooltips[32];
    int count;
} UITooltipManager;

void tooltip_manager_init(
    UITooltipManager *mgr
);

void tooltip_add(
    UITooltipManager *mgr,
    SDL_Rect trigger,
    const char *text
);

void tooltip_update(
    UITooltipManager *mgr,
    SDL_Event *event,
    float dt
);

void tooltip_draw(
    UITooltipManager *mgr,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* TOOLTIPS_H */
