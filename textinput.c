#include "textinput.h"
#include <string.h>
#include <ctype.h>

void textinput_init(
    UITextInput *ti,
    int font_size)
{
    ti->text[0] = '\0';
    ti->cursor = 0;
    ti->sel_start = 0;
    ti->sel_end = 0;
    ti->focused = false;
    ti->hovered = false;
    ti->cursor_blink = 0.0f;
    ti->scroll_x = 0.0f;
    ti->font_size = font_size;
}

void textinput_layout(
    UITextInput *ti,
    UIContext *ui,
    float x, float y,
    float w, float h)
{
    ti->rect =
        ui_rect(ui, x, y, w, h);

    int pad = (int)(12 * ui->scale);

    ti->text_area.x = ti->rect.x + pad;
    ti->text_area.y = ti->rect.y + pad;
    ti->text_area.w =
        ti->rect.w - pad * 2;
    ti->text_area.h =
        ti->rect.h - pad * 2;
}

static void clamp_selection(UITextInput *ti)
{
    int len = (int)strlen(ti->text);

    if (ti->sel_start < 0)
        ti->sel_start = 0;

    if (ti->sel_start > len)
        ti->sel_start = len;

    if (ti->sel_end < 0)
        ti->sel_end = 0;

    if (ti->sel_end > len)
        ti->sel_end = len;
}

static bool has_selection(UITextInput *ti)
{
    return ti->sel_start != ti->sel_end;
}

static void delete_selection(UITextInput *ti)
{
    if (!has_selection(ti))
        return;

    int a = ti->sel_start < ti->sel_end
        ? ti->sel_start
        : ti->sel_end;

    int b = ti->sel_start < ti->sel_end
        ? ti->sel_end
        : ti->sel_start;

    int len = (int)strlen(ti->text);

    memmove(
        ti->text + a,
        ti->text + b,
        len - b + 1
    );

    ti->cursor = a;
    ti->sel_start = a;
    ti->sel_end = a;
}

static void insert_char(
    UITextInput *ti,
    char c)
{
    int len = (int)strlen(ti->text);

    if (len >= TEXTINPUT_MAX - 1)
        return;

    if (has_selection(ti))
        delete_selection(ti);

    len = (int)strlen(ti->text);

    memmove(
        ti->text + ti->cursor + 1,
        ti->text + ti->cursor,
        len - ti->cursor + 1
    );

    ti->text[ti->cursor] = c;
    ti->cursor++;
    ti->sel_start = ti->cursor;
    ti->sel_end = ti->cursor;
}

static void insert_text(
    UITextInput *ti,
    const char *text)
{
    while (*text) {
        if (*text >= 32) {
            insert_char(ti, *text);
        }

        text++;
    }
}

void textinput_event(
    UITextInput *ti,
    SDL_Event *e)
{
    if (e->type == SDL_MOUSEMOTION) {

        ti->hovered =
            ui_point_in_rect(
                e->motion.x,
                e->motion.y,
                ti->rect
            );

        return;
    }

    if (e->type == SDL_MOUSEBUTTONDOWN &&
        e->button.button == SDL_BUTTON_LEFT)
    {
        bool was_focused = ti->focused;

        ti->focused =
            ui_point_in_rect(
                e->button.x,
                e->button.y,
                ti->rect
            );

        if (ti->focused) {

            ti->cursor_blink = 0.0f;

            ti->cursor =
                (int)strlen(ti->text);

            ti->sel_start = ti->cursor;
            ti->sel_end = ti->cursor;

        } else if (was_focused) {

            ti->sel_start = ti->sel_end;
        }

        return;
    }

    if (!ti->focused)
        return;

    if (e->type == SDL_KEYDOWN) {

        bool shift =
            e->key.keysym.mod & KMOD_SHIFT;

        bool ctrl =
            e->key.keysym.mod & KMOD_CTRL;

        SDL_Keycode key =
            e->key.keysym.sym;

        ti->cursor_blink = 0.0f;

        if (key == SDLK_RETURN ||
            key == SDLK_KP_ENTER)
        {
            return;
        }

        if (key == SDLK_BACKSPACE) {

            if (has_selection(ti)) {

                delete_selection(ti);

            } else if (ti->cursor > 0) {

                int len =
                    (int)strlen(ti->text);

                memmove(
                    ti->text +
                        ti->cursor - 1,
                    ti->text +
                        ti->cursor,
                    len - ti->cursor + 1
                );

                ti->cursor--;
                ti->sel_start = ti->cursor;
                ti->sel_end = ti->cursor;
            }

            return;
        }

        if (key == SDLK_DELETE) {

            if (has_selection(ti)) {

                delete_selection(ti);

            } else {

                int len =
                    (int)strlen(ti->text);

                if (ti->cursor < len) {

                    memmove(
                        ti->text +
                            ti->cursor,
                        ti->text +
                            ti->cursor + 1,
                        len - ti->cursor
                    );

                    ti->sel_start =
                        ti->cursor;
                    ti->sel_end =
                        ti->cursor;
                }
            }

            return;
        }

        if (key == SDLK_LEFT) {

            if (shift) {

                ti->sel_end--;

            } else {

                ti->cursor--;

                if (ti->cursor < 0)
                    ti->cursor = 0;

                ti->sel_start = ti->cursor;
                ti->sel_end = ti->cursor;
            }

            clamp_selection(ti);

            return;
        }

        if (key == SDLK_RIGHT) {

            int len =
                (int)strlen(ti->text);

            if (shift) {

                ti->sel_end++;

            } else {

                ti->cursor++;

                if (ti->cursor > len)
                    ti->cursor = len;

                ti->sel_start = ti->cursor;
                ti->sel_end = ti->cursor;
            }

            clamp_selection(ti);

            return;
        }

        if (key == SDLK_HOME) {

            ti->cursor = 0;
            ti->sel_start = ti->cursor;
            ti->sel_end = ti->cursor;

            return;
        }

        if (key == SDLK_END) {

            ti->cursor =
                (int)strlen(ti->text);
            ti->sel_start = ti->cursor;
            ti->sel_end = ti->cursor;

            return;
        }

        if (ctrl && key == SDLK_a) {

            ti->sel_start = 0;
            ti->sel_end =
                (int)strlen(ti->text);
            ti->cursor = ti->sel_end;

            return;
        }

        if (ctrl && key == SDLK_c) {

            if (has_selection(ti)) {

                int a =
                    ti->sel_start <
                    ti->sel_end
                    ? ti->sel_start
                    : ti->sel_end;

                int b =
                    ti->sel_start <
                    ti->sel_end
                    ? ti->sel_end
                    : ti->sel_start;

                char *clip =
                    strndup(
                        ti->text + a,
                        b - a
                    );

                SDL_SetClipboardText(
                    clip
                );

                free(clip);
            }

            return;
        }

        if (ctrl && key == SDLK_x) {

            if (has_selection(ti)) {

                int a =
                    ti->sel_start <
                    ti->sel_end
                    ? ti->sel_start
                    : ti->sel_end;

                int b =
                    ti->sel_start <
                    ti->sel_end
                    ? ti->sel_end
                    : ti->sel_start;

                char *clip =
                    strndup(
                        ti->text + a,
                        b - a
                    );

                SDL_SetClipboardText(
                    clip
                );

                free(clip);

                delete_selection(ti);
            }

            return;
        }

        if (ctrl && key == SDLK_v) {

            char *clip =
                SDL_GetClipboardText();

            if (clip && *clip) {

                if (has_selection(ti))
                    delete_selection(ti);

                insert_text(ti, clip);
            }

            if (clip)
                SDL_free(clip);

            return;
        }

        if (ctrl && key == SDLK_z) {

            ti->text[0] = '\0';
            ti->cursor = 0;
            ti->sel_start = 0;
            ti->sel_end = 0;

            return;
        }

        if (key == SDLK_TAB) {

            insert_char(ti, '\t');

            return;
        }
    }

    if (e->type == SDL_TEXTINPUT) {

        if (has_selection(ti))
            delete_selection(ti);

        insert_text(ti, e->text.text);
    }
}

void textinput_draw(
    UITextInput *ti,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    int r = ui->dark ? 30 : 240;
    int g = ui->dark ? 35 : 245;
    int b = ui->dark ? 55 : 250;

    UIColor bg = ui_theme(
        ui->dark,
        (UIColor){r, g, b, 255},
        (UIColor){r, g, b, 255}
    );

    ui_fill_rounded_rect(
        renderer,
        ti->rect,
        (int)(12 * ui->scale),
        bg
    );

    if (ti->focused || ti->hovered) {

        UIColor border = ui_theme(
            ui->dark,
            (UIColor){80, 130, 230, 200},
            (UIColor){40, 100, 200, 200}
        );

        ui_outline_rounded_rect(
            renderer,
            ti->rect,
            (int)(12 * ui->scale),
            border
        );
    } else {

        UIColor border = ui_theme(
            ui->dark,
            (UIColor){60, 65, 80, 120},
            (UIColor){200, 205, 215, 120}
        );

        ui_outline_rounded_rect(
            renderer,
            ti->rect,
            (int)(12 * ui->scale),
            border
        );
    }

    int text_color_r = ui->dark ? 220 : 30;
    int text_color_g = ui->dark ? 225 : 35;
    int text_color_b = ui->dark ? 240 : 50;

    SDL_Color sdl_text_color = {
        (Uint8)text_color_r,
        (Uint8)text_color_g,
        (Uint8)text_color_b,
        255
    };

    int cx = ti->text_area.x;
    int cy = ti->text_area.y;

    int pad = (int)(4 * ui->scale);

    SDL_Rect clip = ti->text_area;
    clip.x -= pad;
    clip.w += pad * 2;

    SDL_RenderSetClipRect(
        renderer, &clip);

    if (ti->text[0] != '\0') {

        SDL_Surface *surf =
            TTF_RenderUTF8_Blended(
                font,
                ti->text,
                sdl_text_color
            );

        if (surf) {

            SDL_Texture *tex =
                SDL_CreateTextureFromSurface(
                    renderer, surf
                );

            if (tex) {

                SDL_Rect src = {
                    0, 0,
                    surf->w, surf->h
                };

                SDL_Rect dst = {
                    cx, cy,
                    surf->w, surf->h
                };

                if (has_selection(ti)) {

                    int a =
                        ti->sel_start <
                        ti->sel_end
                        ? ti->sel_start
                        : ti->sel_end;

                    int b =
                        ti->sel_start <
                        ti->sel_end
                        ? ti->sel_end
                        : ti->sel_start;

                    char *before =
                        strndup(
                            ti->text,
                            a
                        );

                    char *selected =
                        strndup(
                            ti->text + a,
                            b - a
                        );

                    char *after =
                        strdup(
                            ti->text + b
                        );

                    int bx, by;
                    TTF_SizeUTF8(
                        font,
                        before,
                        &bx, &by
                    );

                    int sx, sy;
                    TTF_SizeUTF8(
                        font,
                        selected,
                        &sx, &sy
                    );

                    SDL_Rect sel_rect = {
                        cx + bx,
                        cy,
                        sx,
                        ti->text_area.h
                    };

                    UIColor sel_bg =
                        ui_theme(
                            ui->dark,
                            (UIColor){
                                60, 100,
                                200, 100
                            },
                            (UIColor){
                                40, 80,
                                180, 100
                            }
                        );

                    SDL_SetRenderDrawColor(
                        renderer,
                        sel_bg.r,
                        sel_bg.g,
                        sel_bg.b,
                        sel_bg.a
                    );

                    SDL_RenderFillRect(
                        renderer,
                        &sel_rect
                    );

                    free(before);
                    free(selected);
                    free(after);
                }

                SDL_RenderCopy(
                    renderer,
                    tex,
                    &src, &dst
                );

                SDL_DestroyTexture(tex);
            }

            SDL_FreeSurface(surf);
        }

    } else if (!ti->focused) {

        UIColor placeholder = ui_theme(
            ui->dark,
            (UIColor){100, 105, 120, 150},
            (UIColor){170, 175, 185, 150}
        );

        ui_text(
            renderer,
            font,
            "Type here...",
            cx,
            cy,
            placeholder
        );
    }

    SDL_RenderSetClipRect(
        renderer, NULL);

    if (ti->focused) {

        ti->cursor_blink += dt;

        if (ti->cursor_blink < 0.5f) {

            char *before =
                strndup(
                    ti->text,
                    ti->cursor
                );

            int bx, by;
            TTF_SizeUTF8(
                font, before, &bx, &by
            );

            SDL_SetRenderDrawColor(
                renderer,
                200, 220, 255, 255
            );

            SDL_Rect cursor_rect = {
                cx + bx,
                cy + (int)(2 * ui->scale),
                (int)(2 * ui->scale),
                ti->text_area.h -
                    (int)(4 * ui->scale)
            };

            SDL_RenderFillRect(
                renderer,
                &cursor_rect
            );

            free(before);
        }

        if (ti->cursor_blink >= 1.0f)
            ti->cursor_blink = 0.0f;
    }
}

const char *textinput_get(UITextInput *ti)
{
    return ti->text;
}

void textinput_set(
    UITextInput *ti,
    const char *text)
{
    snprintf(
        ti->text,
        TEXTINPUT_MAX,
        "%s",
        text
    );

    ti->cursor =
        (int)strlen(ti->text);

    ti->sel_start = ti->cursor;
    ti->sel_end = ti->cursor;
}

void textinput_clear(UITextInput *ti)
{
    ti->text[0] = '\0';
    ti->cursor = 0;
    ti->sel_start = 0;
    ti->sel_end = 0;
}

bool textinput_submit(UITextInput *ti)
{
    if (!ti->focused)
        return false;

    SDL_Event e;
    bool found = false;

    while (SDL_PollEvent(&e)) {

        if (e.type == SDL_KEYDOWN) {

            if (e.key.keysym.sym ==
                SDLK_RETURN ||
                e.key.keysym.sym ==
                SDLK_KP_ENTER)
            {
                found = true;
            }
        }
    }

    return found;
}
