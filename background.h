#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include "ui.h"

void draw_background_base(
    SDL_Renderer *renderer,
    int width,
    int height,
    bool dark
);

void draw_background_orbs(
    SDL_Renderer *renderer,
    int width,
    int height,
    float time,
    bool dark
);

void draw_background(
    SDL_Renderer *renderer,
    int width,
    int height,
    float time,
    bool dark
);

#endif /* BACKGROUND_H */
