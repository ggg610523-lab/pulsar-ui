#include "checkbox.h"

void checkbox_init(
    UICheckBox *checkbox,
    const char *text,
    bool checked)
{
    checkbox->text = text;
    checkbox->checked = checked;
}

void checkbox_layout(
    UICheckBox *checkbox,
    UIContext *ui,
    float x,
    float y)
{
    checkbox->rect =
        ui_rect(
            ui,
            x,
            y,
            260,
            36
        );
}

void checkbox_event(
    UICheckBox *checkbox,
    SDL_Event *event)
{
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
                checkbox->rect
            )
        ) {
            checkbox->checked =
                !checkbox->checked;
        }
    }
}

void checkbox_draw(
    UICheckBox *checkbox,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int size =
        (int)roundf(26.0f * ui->scale);

    if (size < 18)
        size = 18;

    SDL_Rect box = {
        checkbox->rect.x,
        checkbox->rect.y +
            (checkbox->rect.h - size) / 2,
        size,
        size
    };

    ui_glass(
        renderer,
        box,
        (int)roundf(7.0f * ui->scale),
        checkbox->checked,
        ui->dark);

    if (checkbox->checked) {

        SDL_SetRenderDrawBlendMode(
            renderer,
            SDL_BLENDMODE_BLEND);

        /*
            Checkmark glow.
        */
        SDL_SetRenderDrawColor(
            renderer,
            50, 105, 235, 60);

        int gx1 =
            box.x + (int)(size * 0.18f);
        int gy1 =
            box.y + (int)(size * 0.48f);
        int gx2 =
            box.x + (int)(size * 0.45f);
        int gy2 =
            box.y + (int)(size * 0.76f);
        int gx3 =
            box.x + (int)(size * 0.82f);
        int gy3 =
            box.y + (int)(size * 0.24f);

        SDL_RenderDrawLine(
            renderer,
            gx1 - 1, gy1,
            gx2, gy2 + 1);
        SDL_RenderDrawLine(
            renderer,
            gx2, gy2 + 1,
            gx3 + 1, gy3 - 1);
        SDL_RenderDrawLine(
            renderer,
            gx1 + 1, gy1 + 1,
            gx2 + 1, gy2);
        SDL_RenderDrawLine(
            renderer,
            gx2 + 1, gy2,
            gx3, gy3 + 1);

        /*
            Checkmark core (two-pass for thickness).
        */
        SDL_SetRenderDrawColor(
            renderer,
            45, 100, 230, 255);

        int x1 =
            box.x + (int)(size * 0.22f);
        int y1 =
            box.y + (int)(size * 0.52f);
        int x2 =
            box.x + (int)(size * 0.43f);
        int y2 =
            box.y + (int)(size * 0.73f);
        int x3 =
            box.x + (int)(size * 0.78f);
        int y3 =
            box.y + (int)(size * 0.27f);

        SDL_RenderDrawLine(
            renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(
            renderer, x2, y2, x3, y3);

        SDL_SetRenderDrawColor(
            renderer,
            90, 140, 250, 200);

        SDL_RenderDrawLine(
            renderer,
            x1 + 1, y1 - 1,
            x2, y2);
        SDL_RenderDrawLine(
            renderer,
            x2, y2,
            x3 - 1, y3 + 1);
    }

    ui_text(
        renderer,
        font,
        checkbox->text,
        checkbox->rect.x +
            size +
            (int)roundf(10.0f * ui->scale),
        checkbox->rect.y +
            (int)roundf(6.0f * ui->scale),
        ui_theme(ui->dark,
            (UIColor){25, 35, 55, 255},
            (UIColor){195, 205, 225, 255}
        ));
}
