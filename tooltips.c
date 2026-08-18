#include "tooltips.h"
#include <string.h>
#include <math.h>

void tooltip_manager_init(
    UITooltipManager *mgr)
{
    mgr->count = 0;

    for (int i = 0; i < 32; i++) {

        mgr->tooltips[i].text = NULL;
        mgr->tooltips[i].visible = false;
        mgr->tooltips[i].alpha = 0.0f;
        mgr->tooltips[i].timer = 0.0f;
    }
}

void tooltip_add(
    UITooltipManager *mgr,
    SDL_Rect trigger,
    const char *text)
{
    if (mgr->count >= 32)
        return;

    UITooltip *tip =
        &mgr->tooltips[mgr->count];

    tip->triggerRect = trigger;
    tip->text = text;
    tip->visible = false;
    tip->alpha = 0.0f;
    tip->timer = 0.0f;

    mgr->count++;
}

void tooltip_update(
    UITooltipManager *mgr,
    SDL_Event *event,
    float dt)
{
    (void)event;

    int mx = 0;
    int my = 0;

    SDL_GetMouseState(&mx, &my);

    for (int i = 0; i < mgr->count; i++) {

        UITooltip *tip =
            &mgr->tooltips[i];

        SDL_Rect r = tip->triggerRect;

        bool hover =
            mx >= r.x &&
            mx < r.x + r.w &&
            my >= r.y &&
            my < r.y + r.h;

        if (hover) {

            tip->timer += dt;

            if (tip->timer > 0.5f) {
                tip->visible = true;
            }

        } else {

            tip->timer = 0.0f;
            tip->visible = false;
        }

        float target =
            tip->visible ? 1.0f : 0.0f;

        tip->alpha +=
            (target - tip->alpha) *
            8.0f * dt;

        if (tip->alpha < 0.01f)
            tip->alpha = 0.0f;

        if (tip->alpha > 0.99f)
            tip->alpha = 1.0f;
    }
}

void tooltip_draw(
    UITooltipManager *mgr,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    for (int i = 0; i < mgr->count; i++) {

        UITooltip *tip =
            &mgr->tooltips[i];

        if (tip->alpha < 0.01f)
            continue;

        if (!tip->text)
            continue;

        int tw = 0;
        int th = 0;

        TTF_SizeUTF8(
            font,
            tip->text,
            &tw,
            &th
        );

        int pad =
            (int)roundf(10.0f * ui->scale);

        int tipW = tw + pad * 2;
        int tipH = th + pad * 2;

        SDL_Rect r = tip->triggerRect;

        int tipX =
            r.x + r.w / 2 - tipW / 2;

        int tipY =
            r.y - tipH -
            (int)roundf(8.0f * ui->scale);

        if (tipX < 0)
            tipX = 0;

        if (tipY < 0)
            tipY =
                r.y + r.h +
                (int)roundf(8.0f * ui->scale);

        SDL_SetRenderDrawBlendMode(
            renderer,
            SDL_BLENDMODE_BLEND
        );

        int radius =
            (int)roundf(8.0f * ui->scale);

        SDL_Rect tipRect = {
            tipX,
            tipY,
            tipW,
            tipH
        };

        UIColor bg = ui_theme(ui->dark,
            (UIColor){
                35, 40, 55,
                (Uint8)(230 * tip->alpha)},
            (UIColor){
                50, 55, 75,
                (Uint8)(230 * tip->alpha)}
        );

        ui_fill_rounded_rect(
            renderer,
            tipRect,
            radius,
            bg
        );

        UIColor border = ui_theme(ui->dark,
            (UIColor){
                60, 70, 100,
                (Uint8)(120 * tip->alpha)},
            (UIColor){
                80, 90, 120,
                (Uint8)(120 * tip->alpha)}
        );

        ui_outline_rounded_rect(
            renderer,
            tipRect,
            radius,
            border
        );

        UIColor tc = ui_theme(ui->dark,
            (UIColor){
                200, 210, 230,
                (Uint8)(255 * tip->alpha)},
            (UIColor){
                35, 45, 62,
                (Uint8)(255 * tip->alpha)}
        );

        ui_text(
            renderer,
            font,
            tip->text,
            tipX + pad,
            tipY + pad,
            tc
        );
    }
}
