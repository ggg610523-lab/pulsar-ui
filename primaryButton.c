#include "primaryButton.h"

void primary_button_init(
    UIPrimaryButton *button,
    const char *text)
{
    button->text = text;
    button->hovered = false;
    button->pressed = false;
    button->clicked = false;
    button->hover_anim = 0.0f;
}

void primary_button_layout(
    UIPrimaryButton *button,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    button->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            h
        );
}

void primary_button_event(
    UIPrimaryButton *button,
    SDL_Event *event)
{
    if (event->type == SDL_MOUSEMOTION) {

        button->hovered =
            ui_point_in_rect(
                event->motion.x,
                event->motion.y,
                button->rect
            );
    }

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
                button->rect
            )
        ) {
            button->pressed = true;
        }
    }

    if (
        event->type ==
        SDL_MOUSEBUTTONUP &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {

        button->clicked =
            button->pressed &&
            ui_point_in_rect(
                event->button.x,
                event->button.y,
                button->rect
            );

        button->pressed = false;
    }
}

void primary_button_draw(
    UIPrimaryButton *button,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int radius =
        (int)roundf(
            16.0f * ui->scale
        );

    ui_fill_rounded_rect(
        renderer,
        button->rect,
        radius,
        button->pressed
            ? (UIColor){40, 80, 210, 255}
            : button->hovered
                ? (UIColor){60, 110, 240, 255}
                : (UIColor){50, 100, 230, 255}
    );

    ui_outline_rounded_rect(
        renderer,
        button->rect,
        radius,
        ui_theme(ui->dark,
            (UIColor){120, 170, 255, 150},
            (UIColor){80, 130, 220, 150})
    );

    UIColor textColor =
        ui_theme(ui->dark,
            (UIColor){255, 255, 255, 255},
            (UIColor){255, 255, 255, 255}
        );

    ui_text_center(
        renderer,
        font,
        button->text,
        button->rect,
        textColor
    );
}
