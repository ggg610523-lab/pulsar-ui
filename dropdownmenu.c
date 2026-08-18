#include "dropdownmenu.h"
#include <string.h>

void dropdown_init(
    UIDropDown *dd)
{
    dd->itemCount = 0;
    dd->selected = -1;
    dd->open = false;
    dd->hovered = false;
    dd->anim = 0.0f;
}

void dropdown_add_item(
    UIDropDown *dd,
    const char *item)
{
    if (dd->itemCount >= DROPDOWN_MAX_ITEMS)
        return;

    dd->items[dd->itemCount] = item;
    dd->itemCount++;

    if (dd->selected < 0)
        dd->selected = 0;
}

void dropdown_layout(
    UIDropDown *dd,
    UIContext *ui,
    float x,
    float y,
    float w)
{
    dd->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            45
        );

    int itemH = 40;

    dd->listRect =
        ui_rect(
            ui,
            x,
            y + 50,
            w,
            dd->itemCount * itemH + 10
        );
}

void dropdown_event(
    UIDropDown *dd,
    SDL_Event *event)
{
    if (
        event->type ==
        SDL_MOUSEMOTION
    ) {
        dd->hovered =
            ui_point_in_rect(
                event->motion.x,
                event->motion.y,
                dd->rect
            ) ||
            (
                dd->open &&
                ui_point_in_rect(
                    event->motion.x,
                    event->motion.y,
                    dd->listRect
                )
            );
    }

    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {
        int mx = event->button.x;
        int my = event->button.y;

        if (
            ui_point_in_rect(
                mx, my,
                dd->rect
            )
        ) {
            dd->open = !dd->open;
            return;
        }

        if (dd->open) {

            if (
                ui_point_in_rect(
                    mx, my,
                    dd->listRect
                )
            ) {

                int itemH = 40;
                int pad = 5;

                int idx =
                    (my - dd->listRect.y -
                     pad) / itemH;

                if (
                    idx >= 0 &&
                    idx < dd->itemCount
                ) {
                    dd->selected = idx;
                    dd->open = false;
                }

            } else {
                dd->open = false;
            }
        }
    }
}

void dropdown_draw(
    UIDropDown *dd,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    float target = dd->open ? 1.0f : 0.0f;

    dd->anim +=
        (target - dd->anim) *
        10.0f * dt;

    if (dd->anim < 0.005f)
        dd->anim = 0.0f;

    if (dd->anim > 0.995f)
        dd->anim = 1.0f;

    int radius =
        (int)roundf(
            14.0f * ui->scale
        );

    ui_glass(
        renderer,
        dd->rect,
        radius,
        dd->hovered,
        ui->dark
    );

    int padding =
        (int)roundf(14.0f * ui->scale);

    if (dd->selected >= 0 &&
        dd->selected < dd->itemCount)
    {
        ui_text(
            renderer,
            font,
            dd->items[dd->selected],
            dd->rect.x + padding,
            dd->rect.y +
                (dd->rect.h -
                 (int)roundf(
                     19.0f * ui->scale))
                / 2,
            ui_theme(ui->dark,
                (UIColor){30, 40, 58, 255},
                (UIColor){200, 210, 230, 255})
        );
    }

    int arrowX =
        dd->rect.x + dd->rect.w -
        (int)roundf(28.0f * ui->scale);

    int arrowY =
        dd->rect.y + dd->rect.h / 2;

    int aw =
        (int)roundf(6.0f * ui->scale);

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    SDL_SetRenderDrawColor(
        renderer,
        ui->dark ? 155 : 65,
        ui->dark ? 165 : 75,
        ui->dark ? 185 : 95,
        220
    );

    if (dd->open) {

        for (int i = 0; i < aw; i++) {

            SDL_RenderDrawLine(
                renderer,
                arrowX + i,
                arrowY - aw / 2 + i,
                arrowX + aw * 2 - i,
                arrowY - aw / 2 + i
            );
        }

    } else {

        for (int i = 0; i < aw; i++) {

            SDL_RenderDrawLine(
                renderer,
                arrowX + i,
                arrowY + aw / 2 - i,
                arrowX + aw * 2 - i,
                arrowY + aw / 2 - i
            );
        }
    }

    if (dd->anim > 0.01f) {

        int itemH =
            (int)roundf(40.0f * ui->scale);
        int pad =
            (int)roundf(5.0f * ui->scale);

        int listH =
            (int)(dd->itemCount *
                  itemH + pad * 2);

        SDL_Rect listVis =
            ui_rect(
                ui,
                dd->rect.x / ui->scale,
                (dd->rect.y +
                 dd->rect.h + 5) /
                    ui->scale,
                dd->rect.w / ui->scale,
                listH / ui->scale
            );

        ui_glass(
            renderer,
            listVis,
            radius,
            false,
            ui->dark
        );

        for (int i = 0;
             i < dd->itemCount;
             i++)
        {
            SDL_Rect itemRect = {
                listVis.x,
                listVis.y + pad +
                    i * itemH,
                listVis.w,
                itemH
            };

            bool sel = (i == dd->selected);

            if (sel) {
                ui_fill_rounded_rect(
                    renderer,
                    itemRect,
                    radius > 8 ? 8 : radius,
                    ui_theme(ui->dark,
                        (UIColor){
                            230, 235, 245, 180},
                        (UIColor){
                            50, 55, 75, 180})
                );
            }

            ui_text(
                renderer,
                font,
                dd->items[i],
                itemRect.x + padding,
                itemRect.y +
                    (itemRect.h -
                     (int)roundf(
                         19.0f * ui->scale))
                    / 2,
                ui_theme(ui->dark,
                    sel
                        ? (UIColor){
                            30, 85, 210, 255}
                        : (UIColor){
                            30, 40, 58, 255},
                    sel
                        ? (UIColor){
                            100, 160, 255, 255}
                        : (UIColor){
                            200, 210, 230, 255})
            );
        }
    }
}
