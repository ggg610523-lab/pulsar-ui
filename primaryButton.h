#ifndef PRIMARY_BUTTON_H
#define PRIMARY_BUTTON_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;

    const char *text;

    bool hovered;
    bool pressed;
    bool clicked;

    float hover_anim;
} UIPrimaryButton;

void primary_button_init(
    UIPrimaryButton *button,
    const char *text
);

void primary_button_layout(
    UIPrimaryButton *button,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h
);

void primary_button_event(
    UIPrimaryButton *button,
    SDL_Event *event
);

void primary_button_draw(
    UIPrimaryButton *button,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* PRIMARY_BUTTON_H */
