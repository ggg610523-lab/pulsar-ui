#include "sidebar.h"

void sidebar_init(
    UISidebar *sb)
{
    sb->open = false;
    sb->anim = 0.0f;

    sb->items[0] = "Settings";
    sb->items[1] = "Profile";
    sb->items[2] = "About";
    sb->items[3] = "Quit";
}

void sidebar_layout(
    UISidebar *sb,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    sb->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            h
        );
}

void sidebar_event(
    UISidebar *sb,
    SDL_Event *event)
{
    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {

        if (
            sb->open &&
            !ui_point_in_rect(
                event->button.x,
                event->button.y,
                sb->rect
            )
        ) {
            sb->open = false;
        }
    }
}

void sidebar_draw(
    UISidebar *sb,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float dt)
{
    float target = sb->open ? 1.0f : 0.0f;

    sb->anim +=
        (target - sb->anim) *
        6.0f * dt;

    if (sb->anim < 0.005f)
        sb->anim = 0.0f;

    if (sb->anim > 0.995f)
        sb->anim = 1.0f;

    if (sb->anim < 0.001f)
        return;

    float eased =
        ui_ease_out_cubic(sb->anim);

    float maxW =
        (float)sb->rect.w / ui->scale;

    float currentW = eased * maxW;

    SDL_Rect vis =
        ui_rect(
            ui,
            0,
            0,
            currentW,
            sb->rect.h / ui->scale
        );

    ui_glass(
        renderer,
        vis,
        0,
        false,
        ui->dark
    );

    if (sb->anim > 0.4f) {

        float alpha =
            (sb->anim - 0.4f) /
            0.6f;

        if (alpha > 1.0f)
            alpha = 1.0f;

        ui_text(
            renderer,
            font,
            "Menu",
            (int)roundf(25.0f * ui->scale),
            (int)roundf(25.0f * ui->scale),
            (UIColor){
                35, 45, 62,
                (Uint8)(255 * alpha)}
        );

        for (int i = 0;
             i < SIDEBAR_ITEMS;
             i++) {

            ui_text(
                renderer,
                font,
                sb->items[i],
                (int)roundf(
                    25.0f * ui->scale),
                (int)roundf(
                    (70.0f + i * 38.0f) *
                    ui->scale),
                (UIColor){
                    65, 75, 95,
                    (Uint8)(255 * alpha)}
            );
        }
    }
}
