#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "button.h"
#include "primaryButton.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    bool open;
    bool closing;
    bool dark;
    float anim;
    bool dragging;
    int drag_off_x;
    int drag_off_y;
    UIButton cancelBtn;
    UIPrimaryButton okBtn;
} DialogBox;

void dialog_box_init(
    DialogBox *dlg,
    TTF_Font *font
);

void dialog_box_open(
    DialogBox *dlg
);

void dialog_box_event(
    DialogBox *dlg,
    SDL_Event *event
);

void dialog_box_draw(
    DialogBox *dlg,
    bool dark,
    float dt
);

void dialog_box_close(
    DialogBox *dlg
);

#endif /* DIALOG_BOX_H */
