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
            36
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
    int outerRadius =
        (int)roundf(
            10.0f * ui->scale);

    int cx =
        radio->rect.x + outerRadius + 2;

    int cy =
        radio->rect.y + radio->rect.h / 2;

    /*
        Shadow.
    */
    ui_fill_circle(
        renderer,
        cx, cy + 2,
        outerRadius + 1,
        ui_theme(ui->dark,
            (UIColor){15, 20, 40, 20},
            (UIColor){0, 0, 0, 40}));

    /*
        Outer glass circle.
    */
    ui_fill_circle(
        renderer,
        cx, cy,
        outerRadius,
        ui_theme(ui->dark,
            (UIColor){240, 245, 252, 195},
            (UIColor){40, 48, 68, 195}));

    /*
        Bright ring.
    */
    ui_fill_circle(
        renderer,
        cx, cy,
        outerRadius - 1,
        ui_theme(ui->dark,
            (UIColor){255, 255, 255, 110},
            (UIColor){70, 80, 110, 80}));

    /*
        Top highlight arc (simulated).
    */
    if (outerRadius > 4) {

        int hlR = outerRadius - 2;
        int hlY = cy - (int)(hlR * 0.25f);

        SDL_SetRenderDrawBlendMode(
            renderer,
            SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(
            renderer,
            255, 255, 255,
            ui->dark ? 20 : 40);

        for (int y = -hlR; y <= 0; ++y) {

            float fy = (float)y;
            float val =
                (float)(hlR * hlR) - fy * fy;

            if (val <= 0.0f) continue;

            int hw =
                (int)floorf(sqrtf(val));

            SDL_RenderDrawLine(
                renderer,
                cx - hw, hlY + y,
                cx + hw, hlY + y);
        }
    }

    /*
        Selected indicator.
    */
    if (radio->selected) {

        int innerRadius =
            (int)roundf(
                5.0f * ui->scale);

        /* Glow */
        ui_fill_circle(
            renderer,
            cx, cy,
            innerRadius + 3,
            (UIColor){
                55, 110, 240, 40});

        /* Core */
        ui_fill_circle(
            renderer,
            cx, cy,
            innerRadius,
            (UIColor){
                50, 100, 235, 255});

        /* Bright center */
        ui_fill_circle(
            renderer,
            cx, cy,
            innerRadius > 2
                ? innerRadius - 2 : 1,
            (UIColor){
                100, 150, 255, 180});
    }

    ui_text(
        renderer,
        font,
        radio->text,
        radio->rect.x +
            (int)roundf(28.0f * ui->scale),
        radio->rect.y +
            (int)roundf(5.0f * ui->scale),
        ui_theme(ui->dark,
            (UIColor){25, 35, 55, 255},
            (UIColor){195, 205, 225, 255}
        ));
}
