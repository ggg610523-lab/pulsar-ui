
#include "radio.h"

void radio_init(
    UIRadio *radio,
    const char *text,
    bool selected)
{
    radio->text = text;
    radio->selected = selected;
}

void radio_layout(
    UIRadio *radio,
    UIContext *ui,
    float x,
    float y)
{
    radio->rect =
        ui_rect(
            ui,
            x,
            y,
            220,
            32
        );
}

bool radio_event(
    UIRadio *radio,
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
                radio->rect
            )
        ) {
            radio->selected = true;
            return true;
        }
    }

    return false;
}

void radio_draw(
    UIRadio *radio,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int radius =
        (int)(10 * ui->scale);

    int cx =
        radio->rect.x + radius;

    int cy =
        radio->rect.y +
        radio->rect.h / 2;

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        180
    );

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {

            if (x*x + y*y <= radius*radius) {

                SDL_RenderDrawPoint(
                    renderer,
                    cx + x,
                    cy + y
                );
            }
        }
    }

    if (radio->selected) {

        SDL_SetRenderDrawColor(
            renderer,
            60,
            115,
            240,
            255
        );

        int inner = radius / 2;

        for (int y = -inner; y <= inner; y++) {
            for (int x = -inner; x <= inner; x++) {

                if (
                    x*x + y*y <=
                    inner*inner
                ) {
                    SDL_RenderDrawPoint(
                        renderer,
                        cx + x,
                        cy + y
                    );
                }
            }
        }
    }

    ui_text(
        renderer,
        font,
        radio->text,
        radio->rect.x +
            (int)(28 * ui->scale),
        radio->rect.y + 3,
        (UIColor){35, 45, 65, 255}
    );
}
