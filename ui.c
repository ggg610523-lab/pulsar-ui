#include "ui.h"

#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ============================================================
   Internal helpers
   ============================================================ */

static int clamp_int(
    int value,
    int min,
    int max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}


/*
    Calculate the horizontal inset of a rounded rectangle
    at a particular scanline.

    This is the actual quarter-circle geometry.
*/
static float rounded_inset(
    int height,
    int radius,
    int y)
{
    if (radius <= 0)
        return 0.0f;

    if (y >= radius &&
        y < height - radius)
        return 0.0f;

    float r = (float)radius;

    float dy;

    if (y < radius) {

        dy =
            r -
            ((float)y + 0.5f);

    } else {

        dy =
            ((float)y + 0.5f) -
            ((float)height - r);
    }

    float value =
        r * r -
        dy * dy;

    if (value <= 0.0f)
        return r;

    return r - sqrtf(value);
}


UIColor ui_color_lerp(
    UIColor a,
    UIColor b,
    float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    return (UIColor){
        (Uint8)(a.r + (b.r - a.r) * t),
        (Uint8)(a.g + (b.g - a.g) * t),
        (Uint8)(a.b + (b.b - a.b) * t),
        (Uint8)(a.a + (b.a - a.a) * t)
    };
}

UIColor ui_theme(
    bool dark,
    UIColor light,
    UIColor dark_color)
{
    return dark ? dark_color : light;
}


void ui_fill_gradient_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor top,
    UIColor bottom)
{
    if (rect.w <= 0 || rect.h <= 0)
        return;

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    for (int y = 0; y < rect.h; ++y) {

        float t =
            (float)y /
            (float)(rect.h - 1);

        UIColor c =
            ui_color_lerp(top, bottom, t);

        SDL_SetRenderDrawColor(
            renderer,
            c.r,
            c.g,
            c.b,
            c.a
        );

        SDL_RenderDrawLine(
            renderer,
            rect.x,
            rect.y + y,
            rect.x + rect.w - 1,
            rect.y + y
        );
    }
}


void ui_fill_rounded_rect_gradient(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor top,
    UIColor bottom)
{
    if (rect.w <= 0 || rect.h <= 0)
        return;

    int max_radius =
        (rect.w < rect.h ?
            rect.w :
            rect.h) / 2;

    radius =
        clamp_int(radius, 0, max_radius);

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    if (radius == 0) {

        ui_fill_gradient_rect(
            renderer, rect, top, bottom);

        return;
    }

    for (int y = 0; y < rect.h; ++y) {

        float inset =
            rounded_inset(rect.h, radius, y);

        float fl =
            floorf(inset);

        float frac =
            inset - fl;

        int left =
            rect.x + (int)fl;

        int right =
            rect.x + rect.w - (int)fl - 1;

        if (right < left)
            continue;

        float t =
            (float)y /
            (float)(rect.h - 1);

        UIColor c =
            ui_color_lerp(top, bottom, t);

        Uint8 edge_a =
            (Uint8)(
                c.a *
                (1.0f - frac)
            );

        SDL_SetRenderDrawColor(
            renderer,
            c.r,
            c.g,
            c.b,
            c.a
        );

        SDL_RenderDrawLine(
            renderer,
            left + 1,
            rect.y + y,
            right - 1,
            rect.y + y
        );

        SDL_SetRenderDrawColor(
            renderer,
            c.r,
            c.g,
            c.b,
            edge_a
        );

        SDL_RenderDrawPoint(
            renderer,
            left,
            rect.y + y
        );

        SDL_RenderDrawPoint(
            renderer,
            right,
            rect.y + y
        );
    }
}


/* ============================================================
   UI context
   ============================================================ */

void ui_begin(
    UIContext *ui,
    int width,
    int height)
{
    ui->window_w = width;
    ui->window_h = height;

    ui->base_w = 1100.0f;
    ui->base_h = 700.0f;

    float sx =
        (float)width /
        ui->base_w;

    float sy =
        (float)height /
        ui->base_h;

    /*
        Uniform scale.

        This prevents circles becoming ellipses and
        rounded rectangles becoming distorted.
    */
    ui->scale =
        fminf(sx, sy);

    /*
        Allow smaller windows, but prevent the UI
        from becoming completely unusable.
    */
    if (ui->scale < 0.60f)
        ui->scale = 0.60f;

    if (ui->scale > 2.50f)
        ui->scale = 2.50f;
}


/* ============================================================
   Coordinate conversion
   ============================================================ */

SDL_Rect ui_rect(
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    return (SDL_Rect) {
        (int)roundf(x * ui->scale),
        (int)roundf(y * ui->scale),
        (int)roundf(w * ui->scale),
        (int)roundf(h * ui->scale)
    };
}


bool ui_point_in_rect(
    int x,
    int y,
    SDL_Rect rect)
{
    return
        x >= rect.x &&
        y >= rect.y &&
        x < rect.x + rect.w &&
        y < rect.y + rect.h;
}


/* ============================================================
   Rounded rectangle
   ============================================================ */

void ui_fill_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color)
{
    if (rect.w <= 0 || rect.h <= 0)
        return;

    int max_radius =
        (rect.w < rect.h ?
            rect.w :
            rect.h) / 2;

    radius =
        clamp_int(
            radius,
            0,
            max_radius
        );

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

    if (radius == 0) {

        SDL_RenderFillRect(
            renderer,
            &rect
        );

        return;
    }

    /*
        Exact scanline rendering with
        anti-aliased edge pixels.
    */
    for (int y = 0; y < rect.h; ++y) {

        float inset =
            rounded_inset(
                rect.h,
                radius,
                y
            );

        float fl =
            floorf(inset);

        float frac =
            inset - fl;

        int left =
            rect.x + (int)fl;

        int right =
            rect.x +
            rect.w - (int)fl - 1;

        Uint8 edge_a =
            (Uint8)(
                color.a *
                (1.0f - frac)
            );

        if (right >= left) {

            SDL_SetRenderDrawColor(
                renderer,
                color.r,
                color.g,
                color.b,
                color.a
            );

            SDL_RenderDrawLine(
                renderer,
                left + 1,
                rect.y + y,
                right - 1,
                rect.y + y
            );

            SDL_SetRenderDrawColor(
                renderer,
                color.r,
                color.g,
                color.b,
                edge_a
            );

            SDL_RenderDrawPoint(
                renderer,
                left,
                rect.y + y
            );

            SDL_RenderDrawPoint(
                renderer,
                right,
                rect.y + y
            );
        }
    }
}


/* ============================================================
   Rounded outline
   ============================================================ */

void ui_outline_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color)
{
    if (rect.w <= 0 || rect.h <= 0)
        return;

    int max_radius =
        (rect.w < rect.h ?
            rect.w :
            rect.h) / 2;

    radius =
        clamp_int(
            radius,
            0,
            max_radius
        );

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

    /*
        Generate the outline using a mathematical
        rounded rectangle boundary.
    */

    if (radius == 0) {

        SDL_RenderDrawRect(
            renderer,
            &rect
        );

        return;
    }

    const int segments =
        radius * 3 + 12;

    /*
        Top and bottom.
    */
    SDL_RenderDrawLine(
        renderer,
        rect.x + radius,
        rect.y,
        rect.x + rect.w - radius - 1,
        rect.y
    );

    SDL_RenderDrawLine(
        renderer,
        rect.x + radius,
        rect.y + rect.h - 1,
        rect.x + rect.w - radius - 1,
        rect.y + rect.h - 1
    );

    /*
        Left and right.
    */
    SDL_RenderDrawLine(
        renderer,
        rect.x,
        rect.y + radius,
        rect.x,
        rect.y + rect.h - radius - 1
    );

    SDL_RenderDrawLine(
        renderer,
        rect.x + rect.w - 1,
        rect.y + radius,
        rect.x + rect.w - 1,
        rect.y + rect.h - radius - 1
    );

    /*
        Four quarter circles using line segments.
    */

    int prev_tl_x = rect.x;
    int prev_tl_y = rect.y + radius;

    int prev_tr_x = rect.x + rect.w - 1;
    int prev_tr_y = rect.y + radius;

    int prev_br_x = rect.x + rect.w - 1;
    int prev_br_y = rect.y + rect.h - radius - 1;

    int prev_bl_x = rect.x;
    int prev_bl_y = rect.y + rect.h - radius - 1;

    for (int i = 1; i <= segments; ++i) {

        float t =
            (float)i /
            (float)segments;

        /* Top-left */
        float a =
            (float)M_PI +
            t * ((float)M_PI / 2.0f);

        int cx =
            rect.x + radius;
        int cy =
            rect.y + radius;

        int nx =
            (int)roundf(
                cx + cosf(a) * radius);
        int ny =
            (int)roundf(
                cy + sinf(a) * radius);

        SDL_RenderDrawLine(
            renderer,
            prev_tl_x, prev_tl_y,
            nx, ny);

        prev_tl_x = nx;
        prev_tl_y = ny;

        /* Top-right */
        a =
            -((float)M_PI / 2.0f) +
            t * ((float)M_PI / 2.0f);

        cx =
            rect.x + rect.w - radius - 1;
        cy =
            rect.y + radius;

        nx =
            (int)roundf(
                cx + cosf(a) * radius);
        ny =
            (int)roundf(
                cy + sinf(a) * radius);

        SDL_RenderDrawLine(
            renderer,
            prev_tr_x, prev_tr_y,
            nx, ny);

        prev_tr_x = nx;
        prev_tr_y = ny;

        /* Bottom-right */
        a =
            t * ((float)M_PI / 2.0f);

        cx =
            rect.x + rect.w - radius - 1;
        cy =
            rect.y + rect.h - radius - 1;

        nx =
            (int)roundf(
                cx + cosf(a) * radius);
        ny =
            (int)roundf(
                cy + sinf(a) * radius);

        SDL_RenderDrawLine(
            renderer,
            prev_br_x, prev_br_y,
            nx, ny);

        prev_br_x = nx;
        prev_br_y = ny;

        /* Bottom-left */
        a =
            ((float)M_PI / 2.0f) +
            t * ((float)M_PI / 2.0f);

        cx =
            rect.x + radius;
        cy =
            rect.y + rect.h - radius - 1;

        nx =
            (int)roundf(
                cx + cosf(a) * radius);
        ny =
            (int)roundf(
                cy + sinf(a) * radius);

        SDL_RenderDrawLine(
            renderer,
            prev_bl_x, prev_bl_y,
            nx, ny);

        prev_bl_x = nx;
        prev_bl_y = ny;
    }
}


/* ============================================================
   Circle
   ============================================================ */

void ui_fill_circle(
    SDL_Renderer *renderer,
    int cx,
    int cy,
    int radius,
    UIColor color)
{
    if (radius <= 0)
        return;

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

    /*
        Scanline circle.

        This produces a much better circle than
        repeatedly calling RenderDrawPoint().
    */
    for (int y = -radius; y <= radius; ++y) {

        float fy = (float)y;

        float value =
            (float)(radius * radius) -
            fy * fy;

        if (value <= 0.0f)
            continue;

        int halfWidth =
            (int)floorf(
                sqrtf(value)
            );

        SDL_RenderDrawLine(
            renderer,
            cx - halfWidth,
            cy + y,
            cx + halfWidth,
            cy + y
        );
    }
}


/* ============================================================
   Capsule
   ============================================================ */

void ui_fill_capsule(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor color)
{
    if (rect.w <= 0 || rect.h <= 0)
        return;

    /*
        A capsule has a radius equal to half its
        smallest dimension.
    */
    int radius =
        (rect.w < rect.h ?
            rect.w :
            rect.h) / 2;

    ui_fill_rounded_rect(
        renderer,
        rect,
        radius,
        color
    );
}


/* ============================================================
   Radial gradient (soft orbs for backgrounds)
   ============================================================ */

void ui_fill_radial_gradient(
    SDL_Renderer *renderer,
    int cx,
    int cy,
    int radius,
    UIColor center,
    UIColor edge)
{
    if (radius <= 0)
        return;

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND);

    int y0 = cy - radius;
    int y1 = cy + radius;

    if (y0 < 0) y0 = 0;

    for (int y = y0; y <= y1; ++y) {

        float dy = (float)(y - cy);
        float dist =
            fabsf(dy) / (float)radius;

        if (dist > 1.0f) continue;

        float hw_f =
            sqrtf(1.0f - dist * dist) *
            (float)radius;

        int hw = (int)floorf(hw_f);

        UIColor c =
            ui_color_lerp(center, edge, dist);

        SDL_SetRenderDrawColor(
            renderer,
            c.r, c.g, c.b, c.a);

        SDL_RenderDrawLine(
            renderer,
            cx - hw, y,
            cx + hw, y);
    }
}


/* ============================================================
   Easing functions
   ============================================================ */

float ui_ease_out_cubic(float t)
{
    float inv = 1.0f - t;
    return 1.0f - inv * inv * inv;
}

float ui_ease_in_out_cubic(float t)
{
    if (t < 0.5f)
        return 4.0f * t * t * t;

    return 1.0f - (-2.0f * t + 2.0f) *
        (-2.0f * t + 2.0f) *
        (-2.0f * t + 2.0f) / 2.0f;
}


/* ============================================================
   Glass material
   ============================================================ */

void ui_glass(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    bool active,
    bool dark)
{
    if (rect.w < 4 || rect.h < 4)
        return;

    int max_radius =
        (rect.w < rect.h ?
            rect.w :
            rect.h) / 2;

    radius =
        clamp_int(
            radius,
            0,
            max_radius
        );

    /*
        Main fill.
    */

    UIColor material;

    if (dark) {

        material = active
            ? (UIColor){45, 50, 68, 210}
            : (UIColor){30, 35, 50, 175};

    } else {

        material = active
            ? (UIColor){246, 250, 255, 205}
            : (UIColor){240, 245, 252, 175};
    }

    ui_fill_rounded_rect(
        renderer,
        rect,
        radius,
        material
    );

    /*
        Single subtle outline.
    */

    ui_outline_rounded_rect(
        renderer,
        rect,
        radius,
        ui_theme(dark,
            active
            ? (UIColor){255, 255, 255, 150}
            : (UIColor){255, 255, 255, 105},
            active
            ? (UIColor){100, 115, 150, 150}
            : (UIColor){80, 90, 120, 105})
    );
}


/* ============================================================
   Text cache
   ============================================================ */

#define TEXT_CACHE_SIZE 256

typedef struct {
    const char *text;
    TTF_Font *font;
    SDL_Color color;
    SDL_Texture *texture;
    int w;
    int h;
    Uint32 last_used;
} TextCacheEntry;

static TextCacheEntry text_cache[TEXT_CACHE_SIZE];
static int text_cache_count = 0;
static Uint32 text_cache_frame = 0;

static TextCacheEntry *text_cache_find(
    const char *text,
    TTF_Font *font,
    SDL_Color color)
{
    for (int i = 0; i < text_cache_count; ++i) {

        TextCacheEntry *e = &text_cache[i];

        if (e->font == font &&
            e->color.r == color.r &&
            e->color.g == color.g &&
            e->color.b == color.b &&
            e->color.a == color.a &&
            strcmp(e->text, text) == 0) {

            e->last_used = text_cache_frame;

            return e;
        }
    }

    return NULL;
}

static TextCacheEntry *text_cache_evict(void)
{
    int oldest = 0;
    Uint32 oldest_time = UINT32_MAX;

    for (int i = 0; i < text_cache_count; ++i) {

        if (text_cache[i].last_used <
            oldest_time) {

            oldest_time =
                text_cache[i].last_used;

            oldest = i;
        }
    }

    TextCacheEntry *e =
        &text_cache[oldest];

    if (e->texture)
        SDL_DestroyTexture(e->texture);

    e->texture = NULL;
    e->text = NULL;

    return e;
}

static TextCacheEntry *text_cache_insert(
    SDL_Renderer *renderer,
    const char *text,
    TTF_Font *font,
    SDL_Color color)
{
    TextCacheEntry *entry;

    if (text_cache_count < TEXT_CACHE_SIZE) {

        entry =
            &text_cache[text_cache_count++];

    } else {

        entry = text_cache_evict();
    }

    SDL_Surface *surface =
        TTF_RenderUTF8_Blended(
            font, text, color);

    if (!surface)
        return NULL;

    entry->texture =
        SDL_CreateTextureFromSurface(
            renderer, surface);

    if (!entry->texture) {
        SDL_FreeSurface(surface);
        return NULL;
    }

    entry->text = text;
    entry->font = font;
    entry->color = color;
    entry->w = surface->w;
    entry->h = surface->h;
    entry->last_used = text_cache_frame;

    SDL_FreeSurface(surface);

    return entry;
}

void ui_text_cache_clear(void)
{
    for (int i = 0; i < text_cache_count; ++i) {

        if (text_cache[i].texture) {
            SDL_DestroyTexture(
                text_cache[i].texture);
            text_cache[i].texture = NULL;
        }

        text_cache[i].text = NULL;
    }

    text_cache_count = 0;
}

void ui_text_cache_shutdown(void)
{
    ui_text_cache_clear();
}


/* ============================================================
   Text
   ============================================================ */

void ui_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color)
{
    if (!renderer || !font || !text)
        return;

    SDL_Color c = {
        color.r, color.g,
        color.b, color.a
    };

    TextCacheEntry *entry =
        text_cache_find(text, font, c);

    if (!entry) {

        entry = text_cache_insert(
            renderer, text, font, c);
    }

    if (!entry || !entry->texture)
        return;

    SDL_Rect dst = {
        x, y,
        entry->w, entry->h
    };

    SDL_RenderCopy(
        renderer,
        entry->texture,
        NULL,
        &dst
    );

    text_cache_frame++;
}


void ui_text_shadow(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x,
    int y,
    UIColor color,
    UIColor shadow,
    int offset)
{
    /* Draw shadow first (behind) */
    ui_text(
        renderer, font, text,
        x + offset,
        y + offset,
        shadow);

    /* Draw main text */
    ui_text(
        renderer, font, text,
        x, y,
        color);
}


void ui_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    UIColor color)
{
    if (!renderer || !font || !text)
        return;

    SDL_Color c = {
        color.r, color.g,
        color.b, color.a
    };

    TextCacheEntry *entry =
        text_cache_find(text, font, c);

    if (!entry) {

        entry = text_cache_insert(
            renderer, text, font, c);
    }

    if (!entry || !entry->texture)
        return;

    SDL_Rect dst = {
        rect.x + (rect.w - entry->w) / 2,
        rect.y + (rect.h - entry->h) / 2,
        entry->w,
        entry->h
    };

    SDL_RenderCopy(
        renderer,
        entry->texture,
        NULL,
        &dst
    );

    text_cache_frame++;
}
