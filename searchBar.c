#include "searchBar.h"

void searchbar_init(
    UISearchBar *bar)
{
    bar->text[0] = '\0';
    bar->len = 0;
    bar->focused = false;
    bar->cursor_blink = 0.0f;
}

void searchbar_layout(
    UISearchBar *bar,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    bar->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            h
        );
}

void searchbar_event(
    UISearchBar *bar,
    SDL_Event *event)
{
    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {
        bar->focused =
            ui_point_in_rect(
                event->button.x,
                event->button.y,
                bar->rect
            );

        if (bar->focused)
            bar->cursor_blink = 0.0f;
    }

    if (
        event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT
    ) {
        if (
            !ui_point_in_rect(
                event->button.x,
                event->button.y,
                bar->rect)
        ) {
            bar->focused = false;
        }
    }

    if (!bar->focused)
        return;

    if (
        event->type == SDL_KEYDOWN
    ) {
        if (
            event->key.keysym.sym ==
            SDLK_BACKSPACE
        ) {
            if (bar->len > 0) {
                bar->len--;
                bar->text[bar->len] = '\0';
                bar->cursor_blink = 0.0f;
            }
        }
    }

    if (
        event->type == SDL_TEXTINPUT &&
        bar->len < SEARCHBAR_MAX - 1
    ) {
        strcat(bar->text, event->text.text);
        bar->len =
            (int)strlen(bar->text);
        bar->cursor_blink = 0.0f;
    }
}

void searchbar_draw(
    UISearchBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    int radius =
        (int)roundf(
            16.0f * ui->scale
        );

    ui_glass(
        renderer,
        bar->rect,
        radius,
        bar->focused,
        ui->dark
    );

    int padding =
        (int)roundf(14.0f * ui->scale);

    if (bar->len > 0) {

        ui_text(
            renderer,
            font,
            bar->text,
            bar->rect.x + padding,
            bar->rect.y +
                (bar->rect.h -
                 (int)roundf(19.0f * ui->scale))
                / 2,
            ui_theme(ui->dark,
                (UIColor){30, 40, 58, 255},
                (UIColor){200, 210, 230, 255})
        );

    } else {

        ui_text(
            renderer,
            font,
            "Search...",
            bar->rect.x + padding,
            bar->rect.y +
                (bar->rect.h -
                 (int)roundf(19.0f * ui->scale))
                / 2,
            ui_theme(ui->dark,
                (UIColor){140, 155, 180, 255},
                (UIColor){90, 100, 120, 255})
        );
    }

    if (bar->focused) {

        bar->cursor_blink += dt;

        if (fmodf(bar->cursor_blink, 1.0f) <
            0.5f)
        {
            int textW = 0;

            if (bar->len > 0) {

                TTF_SizeUTF8(
                    font,
                    bar->text,
                    &textW,
                    NULL
                );
            }

            int cx =
                bar->rect.x +
                padding +
                textW +
                (int)(2.0f * ui->scale);

            int cy1 =
                bar->rect.y +
                (int)roundf(8.0f * ui->scale);

            int cy2 =
                bar->rect.y +
                bar->rect.h -
                (int)roundf(8.0f * ui->scale);

            SDL_SetRenderDrawBlendMode(
                renderer,
                SDL_BLENDMODE_BLEND);

            SDL_SetRenderDrawColor(
                renderer,
                50, 100, 235, 200);

            SDL_RenderDrawLine(
                renderer,
                cx, cy1,
                cx, cy2);
        }
    }
}
