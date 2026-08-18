#include "menuBar.h"
#include <string.h>
#include <math.h>

void menubar_init(
    UIMenuBar *bar)
{
    bar->menuCount = 0;
    bar->openMenu = -1;
    bar->anim = 0.0f;
}

void menubar_add_menu(
    UIMenuBar *bar,
    const char *label)
{
    if (bar->menuCount >= MENUBAR_MAX_MENUS)
        return;

    bar->menus[bar->menuCount].label = label;
    bar->menus[bar->menuCount].itemCount = 0;
    bar->menuCount++;
}

void menubar_add_item(
    UIMenuBar *bar,
    int menuIndex,
    const char *item)
{
    if (
        menuIndex < 0 ||
        menuIndex >= bar->menuCount)
        return;

    Menu *m = &bar->menus[menuIndex];

    if (m->itemCount >= MENUBAR_MAX_ITEMS)
        return;

    m->items[m->itemCount].label = item;
    m->itemCount++;
}

void menubar_layout(
    UIMenuBar *bar,
    UIContext *ui,
    float x,
    float y,
    float w)
{
    bar->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            40
        );

    float menuW =
        w / (float)bar->menuCount;

    for (int i = 0; i < bar->menuCount; i++) {

        bar->menus[i].rect =
            ui_rect(
                ui,
                x + i * menuW,
                y,
                menuW,
                40
            );

        int itemH = 36;
        int pad = 5;

        float dropW = menuW;

        if (dropW < 140.0f)
            dropW = 140.0f;

        bar->menus[i].menuRect =
            ui_rect(
                ui,
                x + i * menuW,
                y + 42,
                dropW,
                bar->menus[i].itemCount *
                    itemH + pad * 2
            );
    }
}

void menubar_event(
    UIMenuBar *bar,
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

        if (bar->openMenu >= 0) {

            Menu *m =
                &bar->menus[bar->openMenu];

            if (
                mx >= m->menuRect.x &&
                mx < m->menuRect.x +
                    m->menuRect.w &&
                my >= m->menuRect.y &&
                my < m->menuRect.y +
                    m->menuRect.h
            ) {

                int itemH = 36;
                int pad = 5;

                int idx =
                    (my - m->menuRect.y -
                     pad) / itemH;

                if (
                    idx >= 0 &&
                    idx < m->itemCount
                ) {
                    bar->openMenu = -1;
                    bar->anim = 0.0f;
                }

                return;
            }

            bar->openMenu = -1;
            bar->anim = 0.0f;
        }

        for (int i = 0;
             i < bar->menuCount;
             i++)
        {
            SDL_Rect r =
                bar->menus[i].rect;

            if (
                mx >= r.x &&
                mx < r.x + r.w &&
                my >= r.y &&
                my < r.y + r.h
            ) {
                if (bar->openMenu == i) {
                    bar->openMenu = -1;
                    bar->anim = 0.0f;
                } else {
                    bar->openMenu = i;
                    bar->anim = 0.0f;
                }
                return;
            }
        }
    }

    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT &&
        bar->openMenu >= 0
    ) {
        bar->openMenu = -1;
        bar->anim = 0.0f;
    }
}

int menubar_selected(
    UIMenuBar *bar,
    int *menuIdx,
    int *itemIdx)
{
    (void)bar;
    (void)menuIdx;
    (void)itemIdx;
    return 0;
}

void menubar_draw(
    UIMenuBar *bar,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    int radius =
        (int)roundf(
            10.0f * ui->scale
        );

    ui_fill_rounded_rect(
        renderer,
        bar->rect,
        radius,
        ui_theme(ui->dark,
            (UIColor){230, 235, 245, 200},
            (UIColor){35, 40, 58, 200})
    );

    ui_outline_rounded_rect(
        renderer,
        bar->rect,
        radius,
        ui_theme(ui->dark,
            (UIColor){255, 255, 255, 80},
            (UIColor){60, 70, 100, 80})
    );

    for (int i = 0; i < bar->menuCount; i++) {

        SDL_Rect r = bar->menus[i].rect;

        bool hover =
            bar->openMenu == i;

        if (hover) {
            ui_fill_rounded_rect(
                renderer,
                r,
                radius > 6 ? 6 : radius,
                ui_theme(ui->dark,
                    (UIColor){
                        220, 225, 240, 200},
                    (UIColor){
                        50, 55, 75, 200})
            );
        }

        ui_text_center(
            renderer,
            font,
            bar->menus[i].label,
            r,
            ui_theme(ui->dark,
                (UIColor){35, 45, 62, 255},
                (UIColor){180, 190, 210, 255})
        );
    }

    if (bar->openMenu >= 0) {

        float target = 1.0f;

        bar->anim +=
            (target - bar->anim) *
            12.0f * dt;

        if (bar->anim > 0.995f)
            bar->anim = 1.0f;

        float scale =
            ui_ease_out_cubic(bar->anim);

        Menu *m =
            &bar->menus[bar->openMenu];

        int itemH =
            (int)roundf(36.0f * ui->scale);
        int pad =
            (int)roundf(5.0f * ui->scale);

        int listH =
            m->itemCount * itemH + pad * 2;

        SDL_Rect listRect = {
            m->menuRect.x,
            m->menuRect.y,
            m->menuRect.w,
            (int)(listH * scale)
        };

        ui_glass(
            renderer,
            listRect,
            radius,
            false,
            ui->dark
        );

        if (scale > 0.3f) {

            float alpha =
                (scale - 0.3f) / 0.7f;

            for (int i = 0;
                 i < m->itemCount;
                 i++)
            {
                SDL_Rect itemRect = {
                    listRect.x,
                    listRect.y +
                        pad + i * itemH,
                    listRect.w,
                    itemH
                };

                int mx, my;
                SDL_GetMouseState(
                    &mx, &my
                );

                bool hoverItem =
                    mx >= itemRect.x &&
                    mx < itemRect.x +
                        itemRect.w &&
                    my >= itemRect.y &&
                    my < itemRect.y +
                        itemRect.h;

                if (hoverItem) {
                    ui_fill_rounded_rect(
                        renderer,
                        itemRect,
                        6,
                        ui_theme(ui->dark,
                            (UIColor){
                                210, 215,
                                230,
                                (Uint8)(
                                    180 *
                                    alpha)},
                            (UIColor){
                                55, 60,
                                80,
                                (Uint8)(
                                    180 *
                                alpha)})
                    );
                }

                UIColor c = ui_theme(
                    ui->dark,
                    (UIColor){
                        35, 45, 62,
                        (Uint8)(
                            255 * alpha)},
                    (UIColor){
                        180, 190, 210,
                        (Uint8)(
                            255 * alpha)}
                );

                ui_text(
                    renderer,
                    font,
                    m->items[i].label,
                    itemRect.x +
                        (int)roundf(
                            12.0f *
                            ui->scale),
                    itemRect.y +
                        (itemRect.h -
                         (int)roundf(
                             19.0f *
                             ui->scale))
                        / 2,
                    c
                );
            }
        }
    }
}
