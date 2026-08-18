
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
            32
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
    TTF_Font *font,
    float time)
{
    SDL_Rect box = {
        checkbox->rect.x,
        checkbox->rect.y,
        (int)(25 * ui->scale),
        (int)(25 * ui->scale)
    };

    ui_glass(
        renderer,
        box,
        (int)(7 * ui->scale),
        checkbox->checked,
        time
    );

    if (checkbox->checked) {

        SDL_SetRenderDrawColor(
            renderer,
            55,
            110,
            240,
            255
        );

        SDL_RenderDrawLine(
            renderer,
            box.x + 5,
            box.y + 12,
            box.x + 10,
            box.y + 17
        );

        SDL_RenderDrawLine(
            renderer,
            box.x + 10,
            box.y + 17,
            box.x + 20,
            box.y + 7
        );
    }

    ui_text(
        renderer,
        font,
        checkbox->text,
        checkbox->rect.x +
            (int)(36 * ui->scale),
        checkbox->rect.y + 2,
        (UIColor){35, 45, 65, 255}
    );
}
