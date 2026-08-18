#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "ui.h"

typedef struct {
    SDL_Rect rect;

    const char *text;

    bool checked;
} UICheckBox;

void checkbox_init(
    UICheckBox *checkbox,
    const char *text,
    bool checked
);

void checkbox_layout(
    UICheckBox *checkbox,
    UIContext *ui,
    float x,
    float y
);

void checkbox_event(
    UICheckBox *checkbox,
    SDL_Event *event
);

void checkbox_draw(
    UICheckBox *checkbox,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float time
);

#endif /* CHECKBOX_H */
