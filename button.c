#include "button.h"

void button_init(
    UIButton *button,
    const char *text)
{
    button->text = text;
    button->hovered = false;
    button->pressed = false;
    button->clicked = false;
    button->hover_anim = 0.0f;
}

void button_layout(
    UIButton *button,
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

void button_event(
    UIButton *button,
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

void button_draw(
    UIButton *button,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int radius =
        (int)roundf(
            16.0f * ui->scale
        );

    ui_glass(
        renderer,
        button->rect,
        radius,
        button->hovered ||
        button->pressed,
        ui->dark
    );

    UIColor textColor =
        ui_theme(ui->dark,
            button->pressed
            ? (UIColor){30, 85, 210, 255}
            : (UIColor){30, 40, 58, 255},
            button->pressed
            ? (UIColor){100, 160, 255, 255}
            : (UIColor){200, 210, 230, 255}
        );

    ui_text_center(
        renderer,
        font,
        button->text,
        button->rect,
        textColor
    );
}
