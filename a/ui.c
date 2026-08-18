#include "ui.h"

#include <math.h>

static void circle(
    SDL_Renderer *renderer,
    int cx,
    int cy,
    int radius)
{
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(
                    renderer,
                    cx + x,
                    cy + y
                );
            }
        }
    }
}

static void rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect r,
    int radius,
    UIColor color)
{
    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    SDL_Rect a = {
        r.x + radius,
        r.y,
        r.w - radius * 2,
        r.h
    };

    SDL_Rect b = {
        r.x,
        r.y + radius,
        r.w,
        r.h - radius * 2
    };

    SDL_RenderFillRect(renderer, &a);
    SDL_RenderFillRect(renderer, &b);

    circle(
        renderer,
        r.x + radius,
        r.y + radius,
        radius
    );

    circle(
        renderer,
        r.x + r.w - radius - 1,
        r.y + radius,
        radius
    );

    circle(
        renderer,
        r.x + radius,
        r.y + r.h - radius - 1,
        radius
    );

    circle(
        renderer,
        r.x + r.w - radius - 1,
        r.y + r.h - radius - 1,
        radius
    );
}

void ui_begin(
    UIContext *ui,
    int width,
    int height)
{
    ui->window_w = width;
    ui->window_h = height;

    /*
        Base design resolution.

        Everything is scaled relative to this.
    */

    float sx = width / 1100.0f;
    float sy = height / 700.0f;

    ui->scale = fminf(sx, sy);

    if (ui->scale < 0.70f)
        ui->scale = 0.70f;

    if (ui->scale > 1.60f)
        ui->scale = 1.60f;
}

SDL_Rect ui_rect(
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    return (SDL_Rect) {
        (int)(x * ui->scale),
        (int)(y * ui->scale),
        (int)(w * ui->scale),
        (int)(h * ui->scale)
    };
}

bool ui_point_in_rect(
    int x,
    int y,
    SDL_Rect rect)
{
    return
        x >= rect.x &&
        x < rect.x + rect.w &&
        y >= rect.y &&
        y < rect.y + rect.h;
}

void ui_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color)
{
    SDL_Color c = {
        color.r,
        color.g,
        color.b,
        color.a
    };

    SDL_Surface *surface =
        TTF_RenderUTF8_Blended(
            font,
            text,
            c
        );

    if (!surface)
        return;

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {
        x,
        y,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &dst
    );

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void ui_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    UIColor color)
{
    int w;
    int h;

    TTF_SizeUTF8(
        font,
        text,
        &w,
        &h
    );

    ui_text(
        renderer,
        font,
        text,
        rect.x + (rect.w - w) / 2,
        rect.y + (rect.h - h) / 2,
        color
    );
}

void ui_glass(
    SDL_Renderer *renderer,
    SDL_Rect r,
    int radius,
    bool active,
    float time)
{
    /*
        Crisp Liquid Glass.

        There is intentionally NO blur here.

        The material is represented by:

        - translucent body
        - dark lower edge
        - bright upper edge
        - subtle moving reflection
        - shadow
    */

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    /*
        Shadow.
    */

    SDL_Rect shadow = {
        r.x + 2,
        r.y + 5,
        r.w,
        r.h
    };

    rounded_rect(
        renderer,
        shadow,
        radius,
        (UIColor){30, 40, 60, 35}
    );

    /*
        Glass body.
    */

    rounded_rect(
        renderer,
        r,
        radius,
        active
            ? (UIColor){245, 249, 255, 185}
            : (UIColor){240, 245, 252, 155}
    );

    /*
        Inner surface.
    */

    SDL_Rect inner = {
        r.x + 2,
        r.y + 2,
        r.w - 4,
        r.h - 4
    };

    rounded_rect(
        renderer,
        inner,
        radius - 2,
        (UIColor){255, 255, 255, 25}
    );

    /*
        Upper optical edge.
    */

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        active ? 180 : 120
    );

    SDL_Rect top = {
        r.x + radius,
        r.y + 1,
        r.w - radius * 2,
        2
    };

    SDL_RenderFillRect(
        renderer,
        &top
    );

    /*
        Lower optical edge.
    */

    SDL_SetRenderDrawColor(
        renderer,
        80,
        95,
        120,
        50
    );

    SDL_Rect bottom = {
        r.x + radius,
        r.y + r.h - 3,
        r.w - radius * 2,
        2
    };

    SDL_RenderFillRect(
        renderer,
        &bottom
    );

    /*
        Moving reflection.

        This is not blur. It is a crisp,
        low-alpha highlight.
    */

    float wave =
        (sinf(time * 1.4f) + 1.0f) * 0.5f;

    int x =
        r.x + (int)(wave * r.w);

    SDL_Rect reflection = {
        x - 30,
        r.y + 5,
        60,
        r.h - 10
    };

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        active ? 18 : 10
    );

    SDL_RenderFillRect(
        renderer,
        &reflection
    );
}
