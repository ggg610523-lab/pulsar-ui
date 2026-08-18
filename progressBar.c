#include "progressBar.h"
#include <math.h>

static float clamp(
    float value,
    float min,
    float max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

void progress_bar_init(
    UIProgressBar *bar,
    float value)
{
    bar->value = clamp(value, 0.0f, 1.0f);
    bar->displayValue = bar->value;
}

void progress_bar_layout(
    UIProgressBar *bar,
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

void progress_bar_draw(
    UIProgressBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    float dt)
{
    bar->displayValue +=
        (bar->value -
         bar->displayValue) *
        5.0f * dt;

    if (fabsf(
            bar->displayValue -
            bar->value) < 0.001f)
        bar->displayValue = bar->value;

    int radius =
        (int)roundf(
            (float)bar->rect.h / 2.0f
        );

    ui_fill_rounded_rect(
        renderer,
        bar->rect,
        radius,
        ui_theme(ui->dark,
            (UIColor){205, 215, 230, 160},
            (UIColor){35, 42, 58, 160})
    );

    if (bar->displayValue > 0.001f) {

        int fillW =
            (int)roundf(
                bar->displayValue *
                (float)bar->rect.w
            );

        if (fillW < bar->rect.h)
            fillW = bar->rect.h;

        if (fillW > bar->rect.w)
            fillW = bar->rect.w;

        SDL_Rect fill = {
            bar->rect.x,
            bar->rect.y,
            fillW,
            bar->rect.h
        };

        int fillRadius =
            fillW < bar->rect.h
                ? fillW / 2
                : radius;

        ui_fill_rounded_rect_gradient(
            renderer,
            fill,
            fillRadius,
            (UIColor){
                75, 125, 245, 230},
            (UIColor){
                50, 95, 220, 230}
        );
    }
}

void progress_bar_set_value(
    UIProgressBar *bar,
    float value)
{
    bar->value = clamp(value, 0.0f, 1.0f);
}
