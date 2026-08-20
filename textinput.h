#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "ui.h"

#define TEXTINPUT_MAX 512

typedef struct {
    char text[TEXTINPUT_MAX];
    int cursor;
    int sel_start;
    int sel_end;
    bool focused;
    bool hovered;
    float cursor_blink;
    float scroll_x;
    SDL_Rect rect;
    SDL_Rect text_area;
    int font_size;
} UITextInput;

void textinput_init(
    UITextInput *ti,
    int font_size
);

void textinput_layout(
    UITextInput *ti,
    UIContext *ui,
    float x, float y,
    float w, float h
);

void textinput_event(
    UITextInput *ti,
    SDL_Event *e
);

void textinput_draw(
    UITextInput *ti,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt
);

const char *textinput_get(
    UITextInput *ti
);

void textinput_set(
    UITextInput *ti,
    const char *text
);

void textinput_clear(
    UITextInput *ti
);

bool textinput_submit(
    UITextInput *ti
);

#endif /* TEXTINPUT_H */
