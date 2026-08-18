#include "button.h"

void button_init(
    UIButton *button,
    const char *text)
{
    button->text = text;
    button->hovered = false;
    button->pressed = false;
    button->clicked = false;
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
    button->clicked = false;

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
        if (
            button->pressed &&
            ui_point_in_rect(
                event->button.x,
                event->button.y,
                button->rect
            )
        ) {
            button->clicked = true;
        }

        button->pressed = false;
    }
}

void button_draw(
    UIButton *button,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float time)
{
    (void)ui;

    ui_glass(
        renderer,
        button->rect,
        14,
        button->hovered || button->pressed,
        time
    );

    ui_text_center(
        renderer,
        font,
        button->text,
        button->rect,
        (UIColor){35, 45, 65, 255}
    );
}
