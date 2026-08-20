#include "tabs.h"
#include <string.h>
#include <math.h>

void tabs_init(
    UITabs *tabs)
{
    tabs->count = 0;
    tabs->selected = 0;
    tabs->indicatorX = 0.0f;
    tabs->indicatorW = 0.0f;
}

void tabs_add(
    UITabs *tabs,
    const char *label)
{
    if (tabs->count >= TABS_MAX)
        return;

    tabs->labels[tabs->count] = label;
    tabs->count++;
}

void tabs_layout(
    UITabs *tabs,
    UIContext *ui,
    float x,
    float y,
    float w)
{
    tabs->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            45
        );

    float tabW =
        w / (float)tabs->count;

    for (int i = 0; i < tabs->count; i++) {

        tabs->tabRects[i] =
            ui_rect(
                ui,
                x + i * tabW,
                y,
                tabW,
                45
            );
    }
}

void tabs_event(
    UITabs *tabs,
    SDL_Event *event)
{
    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {

        int mx = event->button.x;
        int my = event->button.y;

        for (int i = 0; i < tabs->count; i++) {

            SDL_Rect r = tabs->tabRects[i];

            if (
                mx >= r.x &&
                mx < r.x + r.w &&
                my >= r.y &&
                my < r.y + r.h
            ) {
                tabs->selected = i;
                return;
            }
        }
    }
}

void tabs_draw(
    UITabs *tabs,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    if (tabs->count == 0)
        return;

    int radius =
        (int)roundf(
            14.0f * ui->scale
        );

    ui_fill_rounded_rect(
        renderer,
        tabs->rect,
        radius,
        ui_theme(ui->dark,
            (UIColor){205, 215, 230, 120},
            (UIColor){30, 36, 52, 120})
    );

    SDL_Rect sel =
        tabs->tabRects[tabs->selected];

    float targetX = (float)sel.x;
    float targetW = (float)sel.w;

    tabs->indicatorX +=
        (targetX - tabs->indicatorX) *
        8.0f * dt;

    tabs->indicatorW +=
        (targetW - tabs->indicatorW) *
        8.0f * dt;

    SDL_Rect indicator = {
        (int)roundf(tabs->indicatorX),
        tabs->rect.y,
        (int)roundf(tabs->indicatorW),
        tabs->rect.h
    };

    ui_fill_rounded_rect(
        renderer,
        indicator,
        radius,
        ui_theme(ui->dark,
            (UIColor){240, 245, 252, 200},
            (UIColor){45, 52, 72, 200})
    );

    ui_outline_rounded_rect(
        renderer,
        indicator,
        radius,
        ui_theme(ui->dark,
            (UIColor){255, 255, 255, 100},
            (UIColor){35, 40, 60, 55})
    );

    for (int i = 0; i < tabs->count; i++) {

        SDL_Rect r = tabs->tabRects[i];

        bool sel = (i == tabs->selected);

        ui_text_center(
            renderer,
            font,
            tabs->labels[i],
            r,
            ui_theme(ui->dark,
                sel
                    ? (UIColor){
                        30, 85, 210, 255}
                    : (UIColor){
                        65, 75, 95, 255},
                sel
                    ? (UIColor){
                        100, 160, 255, 255}
                    : (UIColor){
                        155, 165, 185, 255})
        );
    }
}
