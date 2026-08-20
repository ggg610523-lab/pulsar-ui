#include "toggle.h"
#include <math.h>

void toggle_init(
    UIToggle *toggle,
    bool on)
{
    toggle->on = on;
    toggle->clicked = false;
    toggle->thumb_x = on ? 1.0f : 0.0f;
    toggle->anim = on ? 1.0f : 0.0f;
}

void toggle_layout(
    UIToggle *toggle,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    toggle->rect =
        ui_rect(ui, x, y, w, h);
}

void toggle_event(
    UIToggle *toggle,
    SDL_Event *event)
{
    toggle->clicked = false;

    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {
        if (
            ui_point_in_rect(
                event->button.x,
                event->button.y,
                toggle->rect
            )
        ) {
            toggle->on = !toggle->on;
            toggle->clicked = true;
        }
    }
}

void toggle_draw(
    UIToggle *toggle,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    float target = toggle->on ? 1.0f : 0.0f;
    float diff = target - toggle->anim;

    if (fabsf(diff) < 0.001f)
        toggle->anim = target;
    else
        toggle->anim += diff * 0.25f;

    toggle->thumb_x = toggle->anim;

    int rx = toggle->rect.x;
    int ry = toggle->rect.y;
    int rw = toggle->rect.w;
    int rh = toggle->rect.h;

    int radius = rh / 2;

    SDL_Rect track = {
        rx, ry, rw, rh
    };

    UIColor trackColor =
        ui_theme(ui->dark,
            toggle->on
                ? (UIColor){80, 160, 255, 255}
                : (UIColor){200, 205, 215, 255},
            toggle->on
                ? (UIColor){70, 140, 240, 255}
                : (UIColor){55, 60, 80, 255}
        );

    ui_fill_rounded_rect(
        renderer,
        track,
        radius,
        trackColor
    );

    if (toggle->on) {

        ui_fill_rounded_rect(
            renderer,
            track,
            radius,
            (UIColor){255, 255, 255, 25}
        );
    }

    float thumbPad = 3.0f * ui->scale;
    float thumbR =
        (float)radius - thumbPad;

    float thumbCX =
        (float)rx + thumbPad + thumbR +
        toggle->thumb_x *
        ((float)(rw - 2 * (int)thumbPad) -
         2.0f * thumbR);

    float thumbCY =
        (float)ry + (float)rh * 0.5f;

    ui_fill_circle(
        renderer,
        (int)roundf(thumbCX),
        (int)roundf(thumbCY),
        (int)roundf(thumbR),
        (UIColor){255, 255, 255, 255}
    );

    if (thumbR > 4.0f) {

        int hlR = (int)roundf(thumbR - 1.5f);
        int hlY =
            (int)roundf(thumbCY) -
            (int)(hlR * 0.3f);

        SDL_SetRenderDrawBlendMode(
            renderer,
            SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(
            renderer,
            255, 255, 255, 60);

        for (int y = -hlR; y <= 0; ++y) {

            float fy = (float)y;
            float val =
                (float)(hlR * hlR) -
                fy * fy;

            if (val <= 0.0f) continue;

            int hw =
                (int)floorf(sqrtf(val));

            SDL_RenderDrawLine(
                renderer,
                (int)roundf(thumbCX) - hw,
                hlY + y,
                (int)roundf(thumbCX) + hw,
                hlY + y);
        }
    }

    ui_text(
        renderer,
        font,
        toggle->on ? "Dark" : "Light",
        rx + rw + (int)(12.0f * ui->scale),
        ry + (int)(5.0f * ui->scale),
        ui_theme(ui->dark,
            (UIColor){25, 35, 55, 255},
            (UIColor){195, 205, 225, 255}
        ));
}
