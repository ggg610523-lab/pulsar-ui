#ifndef RADIO_H
#define RADIO_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;

    const char *text;

    bool selected;
} UIRadio;

void radio_init(
    UIRadio *radio,
    const char *text,
    bool selected
);

void radio_layout(
    UIRadio *radio,
    UIContext *ui,
    float x,
    float y
);

bool radio_event(
    UIRadio *radio,
    SDL_Event *event
);

void radio_draw(
    UIRadio *radio,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* RADIO_H */
