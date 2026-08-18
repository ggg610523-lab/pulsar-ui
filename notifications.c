#include "notifications.h"
#include <string.h>
#include <math.h>

void notif_manager_init(
    NotificationManager *mgr)
{
    mgr->count = 0;

    for (int i = 0; i < NOTIF_MAX; i++) {

        mgr->items[i].text[0] = '\0';
        mgr->items[i].alpha = 0.0f;
        mgr->items[i].timer = 0.0f;
        mgr->items[i].active = false;
    }
}

void notif_push(
    NotificationManager *mgr,
    const char *text)
{
    Notification *n =
        &mgr->items[0];

    strncpy(
        n->text,
        text,
        NOTIF_TEXT_MAX - 1
    );

    n->text[NOTIF_TEXT_MAX - 1] = '\0';
    n->alpha = 0.0f;
    n->timer = 2.5f;
    n->active = true;
}

void notif_update(
    NotificationManager *mgr,
    float dt)
{
    for (int i = 0; i < NOTIF_MAX; i++) {

        Notification *n =
            &mgr->items[i];

        if (!n->active)
            continue;

        n->timer -= dt;

        if (n->timer <= 0.0f) {
            n->active = false;
            n->alpha = 0.0f;
            continue;
        }

        if (n->timer > 1.5f) {

            n->alpha += 5.0f * dt;

            if (n->alpha > 1.0f)
                n->alpha = 1.0f;

        } else if (n->timer > 0.3f) {

            n->alpha = 1.0f;

        } else {

            n->alpha -= 4.0f * dt;

            if (n->alpha < 0.0f)
                n->alpha = 0.0f;
        }
    }
}

void notif_draw(
    NotificationManager *mgr,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int padX =
        (int)roundf(15.0f * ui->scale);

    int gap =
        (int)roundf(10.0f * ui->scale);

    int notifH =
        (int)roundf(45.0f * ui->scale);

    int notifW =
        (int)roundf(250.0f * ui->scale);

    int startX =
        ui->window_w - notifW - padX;

    int startY = padX;

    for (int i = 0; i < NOTIF_MAX; i++) {

        Notification *n =
            &mgr->items[i];

        if (!n->active)
            continue;

        if (n->alpha < 0.01f)
            continue;

        SDL_Rect rect = {
            startX,
            startY,
            notifW,
            notifH
        };

        int radius =
            (int)roundf(
                12.0f * ui->scale
            );

        Uint8 a =
            (Uint8)(220 * n->alpha);

        ui_fill_rounded_rect(
            renderer,
            rect,
            radius,
            ui_theme(ui->dark,
                (UIColor){
                    240, 245, 252, a},
                (UIColor){
                    40, 48, 68, a})
        );

        Uint8 ba =
            (Uint8)(100 * n->alpha);

        ui_outline_rounded_rect(
            renderer,
            rect,
            radius,
            ui_theme(ui->dark,
                (UIColor){
                    255, 255, 255, ba},
                (UIColor){
                    80, 90, 120, ba})
        );

        int textPad =
            (int)roundf(14.0f * ui->scale);

        ui_text(
            renderer,
            font,
            n->text,
            rect.x + textPad,
            rect.y +
                (rect.h -
                 (int)roundf(
                     19.0f * ui->scale))
                / 2,
            ui_theme(ui->dark,
                (UIColor){
                    35, 45, 62,
                    (Uint8)(255 * n->alpha)},
                (UIColor){
                    195, 205, 225,
                    (Uint8)(255 * n->alpha)})
        );

        startY += notifH + gap;
    }
}
