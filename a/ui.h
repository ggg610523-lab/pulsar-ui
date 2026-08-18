#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float w;
    float h;
} UIFloatRect;

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} UIColor;

typedef struct {
    float scale;
    int window_w;
    int window_h;
} UIContext;

void ui_begin(
    UIContext *ui,
    int width,
    int height
);

SDL_Rect ui_rect(
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void ui_glass(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    bool active,
    float time
);

void ui_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color
);

void ui_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    UIColor color
);

bool ui_point_in_rect(
    int x,
    int y,
    SDL_Rect rect
);

#endif /* UI_H */
