#ifndef ORB_H
#define ORB_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define ORB_RES 128

typedef struct {
    SDL_Texture *texture;
    Uint32 pixels[ORB_RES * ORB_RES];
    float time;
    float audio;
    bool dirty;
    bool visible;
    float dx, dy, dw, dh;
    SDL_Rect rect;
} Orb;

void orb_init(
    Orb *orb,
    SDL_Renderer *renderer
);

void orb_free(Orb *orb);

void orb_update(
    Orb *orb,
    float dt
);

void orb_draw(
    Orb *orb,
    SDL_Renderer *renderer,
    SDL_Rect dst
);

#endif /* ORB_H */
