#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

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

    /*
        Base design resolution.
    */
    float base_w;
    float base_h;

    bool dark;
} UIContext;

/* ---------------------------------------------------------
   Context
   --------------------------------------------------------- */

void ui_begin(
    UIContext *ui,
    int width,
    int height
);

/* ---------------------------------------------------------
   Geometry
   --------------------------------------------------------- */

SDL_Rect ui_rect(
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

bool ui_point_in_rect(
    int x,
    int y,
    SDL_Rect rect
);

/* ---------------------------------------------------------
   Primitive rendering
   --------------------------------------------------------- */

void ui_fill_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color
);

void ui_outline_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color
);

void ui_fill_circle(
    SDL_Renderer *renderer,
    int cx,
    int cy,
    int radius,
    UIColor color
);

void ui_fill_capsule(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor color
);

/* ---------------------------------------------------------
   Materials
   --------------------------------------------------------- */

void ui_glass(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    bool active,
    bool dark
);

/* ---------------------------------------------------------
   Gradient fill
   --------------------------------------------------------- */

void ui_fill_rounded_rect_gradient(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor top,
    UIColor bottom
);

void ui_fill_gradient_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor top,
    UIColor bottom
);

UIColor ui_color_lerp(
    UIColor a,
    UIColor b,
    float t
);

UIColor ui_theme(
    bool dark,
    UIColor light,
    UIColor dark_color
);

/* ---------------------------------------------------------
   Radial gradient (for soft orbs)
   --------------------------------------------------------- */

void ui_fill_radial_gradient(
    SDL_Renderer *renderer,
    int cx,
    int cy,
    int radius,
    UIColor center,
    UIColor edge
);

/* ---------------------------------------------------------
   Easing
   --------------------------------------------------------- */

float ui_ease_out_cubic(float t);
float ui_ease_in_out_cubic(float t);

/* ---------------------------------------------------------
   Text (cached, with optional shadow)
   --------------------------------------------------------- */

void ui_text_cache_clear(void);
void ui_text_cache_shutdown(void);

void ui_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color
);

void ui_text_shadow(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color,
    UIColor shadow,
    int offset
);

void ui_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    UIColor color
);

#endif /* UI_H */
