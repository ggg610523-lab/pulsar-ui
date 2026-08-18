#include "dialogBox.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static void draw_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    SDL_Color color)
{
    SDL_Surface *surface =
        TTF_RenderUTF8_Blended(
            font,
            text,
            color
        );

    if (!surface)
        return;

    SDL_Texture *tex =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    if (tex) {
        SDL_Rect dst =
            { x, y, surface->w, surface->h };

        SDL_RenderCopy(
            renderer,
            tex,
            NULL,
            &dst
        );

        SDL_DestroyTexture(tex);
    }

    SDL_FreeSurface(surface);
}

static void draw_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    SDL_Color color)
{
    int w = 0;
    int h = 0;

    TTF_SizeUTF8(font, text, &w, &h);

    draw_text(
        renderer,
        font,
        text,
        rect.x + (rect.w - w) / 2,
        rect.y + (rect.h - h) / 2,
        color
    );
}

void dialog_box_init(
    DialogBox *dlg,
    TTF_Font *font)
{
    dlg->window = NULL;
    dlg->renderer = NULL;
    dlg->font = font;
    dlg->open = false;
    dlg->closing = false;
    dlg->dark = false;
    dlg->anim = 0.0f;
    dlg->dragging = false;
    dlg->drag_off_x = 0;
    dlg->drag_off_y = 0;

    button_init(
        &dlg->cancelBtn,
        "Cancel"
    );

    primary_button_init(
        &dlg->okBtn,
        "OK"
    );
}

void dialog_box_open(
    DialogBox *dlg)
{
    if (dlg->open)
        return;

    dlg->window =
        SDL_CreateWindow(
            "Dialog",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            360,
            200,
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_SKIP_TASKBAR
        );

    if (!dlg->window)
        return;

    SDL_SetWindowBordered(
        dlg->window,
        SDL_FALSE
    );

    dlg->renderer =
        SDL_CreateRenderer(
            dlg->window,
            -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC
        );

    if (!dlg->renderer) {
        SDL_DestroyWindow(dlg->window);
        dlg->window = NULL;
        return;
    }

    SDL_SetHint(
        SDL_HINT_RENDER_SCALE_QUALITY,
        "1"
    );

    dlg->cancelBtn.rect =
        (SDL_Rect){ 30, 140, 140, 45 };

    dlg->okBtn.rect =
        (SDL_Rect){ 190, 140, 140, 45 };

    dlg->open = true;
    dlg->closing = false;
    dlg->anim = 0.0f;
    dlg->dragging = false;
}

void dialog_box_event(
    DialogBox *dlg,
    SDL_Event *event)
{
    if (!dlg->open || dlg->closing)
        return;

    Uint32 winID =
        SDL_GetWindowID(dlg->window);

    if (
        event->type ==
        SDL_WINDOWEVENT &&
        event->window.event ==
        SDL_WINDOWEVENT_CLOSE &&
        event->window.windowID == winID
    ) {
        dialog_box_close(dlg);
        return;
    }

    if (
        event->type ==
        SDL_KEYDOWN &&
        event->key.windowID == winID
    ) {
        if (
            event->key.keysym.sym ==
            SDLK_ESCAPE
        ) {
            dialog_box_close(dlg);
            return;
        }
    }

    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.windowID == winID &&
        event->button.button ==
            SDL_BUTTON_LEFT
    ) {
        int mx = event->button.x;
        int my = event->button.y;

        SDL_Rect cr = dlg->cancelBtn.rect;
        SDL_Rect okr = dlg->okBtn.rect;

        bool onBtn =
            (mx >= cr.x &&
             mx < cr.x + cr.w &&
             my >= cr.y &&
             my < cr.y + cr.h) ||
            (mx >= okr.x &&
             mx < okr.x + okr.w &&
             my >= okr.y &&
             my < okr.y + okr.h);

        if (!onBtn && my < 130) {
            dlg->dragging = true;
            dlg->drag_off_x = mx;
            dlg->drag_off_y = my;
            return;
        }

        if (
            mx >= cr.x &&
            mx < cr.x + cr.w &&
            my >= cr.y &&
            my < cr.y + cr.h
        ) {
            dialog_box_close(dlg);
            return;
        }

        if (
            mx >= okr.x &&
            mx < okr.x + okr.w &&
            my >= okr.y &&
            my < okr.y + okr.h
        ) {
            dialog_box_close(dlg);
            return;
        }
    }

    if (
        event->type ==
        SDL_MOUSEBUTTONUP &&
        event->button.windowID == winID &&
        event->button.button ==
            SDL_BUTTON_LEFT
    ) {
        dlg->dragging = false;
    }

    if (
        dlg->dragging &&
        event->type ==
        SDL_MOUSEMOTION &&
        event->motion.windowID == winID
    ) {
        int wx, wy;
        SDL_GetWindowPosition(
            dlg->window,
            &wx,
            &wy
        );

        SDL_SetWindowPosition(
            dlg->window,
            wx + event->motion.xrel,
            wy + event->motion.yrel
        );
    }
}

static void destroy_dialog(
    DialogBox *dlg)
{
    if (dlg->renderer) {
        SDL_DestroyRenderer(dlg->renderer);
        dlg->renderer = NULL;
    }

    if (dlg->window) {
        SDL_DestroyWindow(dlg->window);
        dlg->window = NULL;
    }

    dlg->open = false;
    dlg->closing = false;
    dlg->anim = 0.0f;
}

void dialog_box_draw(
    DialogBox *dlg,
    bool dark,
    float dt)
{
    if (!dlg->open)
        return;

    dlg->dark = dark;

    if (dlg->closing) {

        dlg->anim -= 10.0f * dt;

        if (dlg->anim <= 0.0f) {
            destroy_dialog(dlg);
            return;
        }

    } else {

        dlg->anim +=
            (1.0f - dlg->anim) *
            10.0f * dt;

        if (dlg->anim > 0.995f)
            dlg->anim = 1.0f;
    }

    float scale =
        ui_ease_out_cubic(dlg->anim);

    SDL_SetRenderTarget(
        dlg->renderer,
        NULL
    );

    SDL_SetRenderDrawColor(
        dlg->renderer,
        dark ? 18 : 205,
        dark ? 20 : 215,
        dark ? 35 : 242,
        255
    );

    SDL_RenderClear(dlg->renderer);

    if (scale < 0.01f) {
        SDL_RenderPresent(dlg->renderer);
        return;
    }

    int cx = 180;
    int cy = 100;
    int w = (int)(360 * scale);
    int h = (int)(200 * scale);

    SDL_Rect panelRect =
        { cx - w / 2, cy - h / 2, w, h };

    ui_glass(
        dlg->renderer,
        panelRect,
        (int)(20 * scale),
        false,
        dark
    );

    if (scale > 0.4f) {

        float textAlpha =
            (scale - 0.4f) / 0.6f;

        if (textAlpha > 1.0f)
            textAlpha = 1.0f;

        SDL_Color titleColor = dark
            ? (SDL_Color){
                180, 190, 210,
                (Uint8)(255 * textAlpha)}
            : (SDL_Color){
                35, 45, 62,
                (Uint8)(255 * textAlpha)};

        draw_text(
            dlg->renderer,
            dlg->font,
            "Dialog",
            panelRect.x +
                (int)(30 * scale),
            panelRect.y +
                (int)(30 * scale),
            titleColor
        );

        int btnY =
            panelRect.y +
            panelRect.h -
            (int)(60 * scale);
        int btnH =
            (int)(45 * scale);
        int btnW =
            (int)(140 * scale);
        int pad =
            (int)(30 * scale);
        int gap =
            (int)(20 * scale);

        SDL_Rect cancelRect =
            { panelRect.x + pad,
              btnY,
              btnW,
              btnH };

        SDL_Rect okRect =
            { panelRect.x + pad +
                btnW + gap,
              btnY,
              btnW,
              btnH };

        dlg->cancelBtn.rect = cancelRect;
        dlg->okBtn.rect = okRect;

        ui_glass(
            dlg->renderer,
            cancelRect,
            (int)(14 * scale),
            false,
            dark
        );

        SDL_Color cancelColor = dark
            ? (SDL_Color){
                155, 165, 185,
                (Uint8)(255 * textAlpha)}
            : (SDL_Color){
                65, 75, 95,
                (Uint8)(255 * textAlpha)};

        draw_text_center(
            dlg->renderer,
            dlg->font,
            "Cancel",
            cancelRect,
            cancelColor
        );

        ui_fill_rounded_rect(
            dlg->renderer,
            okRect,
            (int)(14 * scale),
            (UIColor){
                50, 100, 230,
                (Uint8)(255 * textAlpha)}
        );

        ui_outline_rounded_rect(
            dlg->renderer,
            okRect,
            (int)(14 * scale),
            (UIColor){
                120, 170, 255,
                (Uint8)(150 * textAlpha)}
        );

        draw_text_center(
            dlg->renderer,
            dlg->font,
            "OK",
            okRect,
            (SDL_Color){
                255, 255, 255,
                (Uint8)(255 * textAlpha)}
        );
    }

    SDL_RenderPresent(dlg->renderer);
}

void dialog_box_close(
    DialogBox *dlg)
{
    if (!dlg->open || dlg->closing)
        return;

    dlg->closing = true;
}
