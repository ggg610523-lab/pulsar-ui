#include "hamburger.h"

void hamburger_init(
    UIHamburger *hb)
{
    hb->open = false;
    hb->clicked = false;
    hb->anim = 0.0f;
}

void hamburger_layout(
    UIHamburger *hb,
    UIContext *ui,
    float x,
    float y,
    float size)
{
    hb->rect =
        ui_rect(
            ui,
            x,
            y,
            size,
            size
        );
}

void hamburger_event(
    UIHamburger *hb,
    SDL_Event *event)
{
    hb->clicked = false;

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
                hb->rect
            )
        ) {
            hb->open = !hb->open;
            hb->clicked = true;
        }
    }
}

void hamburger_draw(
    UIHamburger *hb,
    UIContext *ui,
    SDL_Renderer *renderer)
{
    int radius =
        (int)roundf(
            12.0f * ui->scale
        );

    ui_glass(
        renderer,
        hb->rect,
        radius,
        hb->open,
        ui->dark
    );

    int cx =
        hb->rect.x + hb->rect.w / 2;

    int lineW =
        (int)roundf(16.0f * ui->scale);

    int gap =
        (int)roundf(5.0f * ui->scale);

    int lw =
        (int)roundf(2.0f * ui->scale);

    if (lw < 2)
        lw = 2;

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(
        renderer,
        50, 60, 85, 220);

    for (int i = -1; i <= 1; i++) {

        int ly =
            hb->rect.y +
            hb->rect.h / 2 +
            i * gap;

        SDL_RenderDrawLine(
            renderer,
            cx - lineW / 2,
            ly,
            cx + lineW / 2,
            ly
        );

        if (lw > 2) {

            SDL_RenderDrawLine(
                renderer,
                cx - lineW / 2,
                ly + 1,
                cx + lineW / 2,
                ly + 1
            );
        }
    }
}
