#include "orb.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

static float hash21(float px, float py)
{
    float x = px * 123.34f + py * 456.21f;
    x = x - floorf(x);

    float y = x + 45.32f;
    y = py + y;
    y = y - floorf(y);

    return x * y - floorf(x * y);
}

static float noise2d(float x, float y)
{
    float ix = floorf(x);
    float iy = floorf(y);

    float fx = x - ix;
    float fy = y - iy;

    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);

    float a = hash21(ix, iy);
    float b = hash21(ix + 1.0f, iy);
    float c = hash21(ix, iy + 1.0f);
    float d = hash21(ix + 1.0f, iy + 1.0f);

    float ab = a + (b - a) * fx;
    float cd = c + (d - c) * fx;

    return ab + (cd - ab) * fy;
}

static float fbm(float x, float y)
{
    float value = 0.0f;
    float amplitude = 0.5f;

    for (int i = 0; i < 5; i++) {

        value += noise2d(x, y) * amplitude;

        x *= 2.0f;
        y *= 2.0f;

        amplitude *= 0.5f;
    }

    return value;
}

static void palette(
    float t,
    float *r, float *g, float *b)
{
    t = t - floorf(t);

    float mr = 1.00f, mg = 0.02f, mb = 0.42f;
    float pr = 0.55f, pg = 0.05f, pb = 1.00f;
    float br = 0.04f, bg2 = 0.20f, bb = 1.00f;
    float cr = 0.00f, cg = 0.85f, cb = 1.00f;
    float pir = 1.00f, pig = 0.30f, pib = 0.75f;

    if (t < 0.20f) {

        float s = t / 0.20f;

        *r = mr + (pr - mr) * s;
        *g = mg + (pg - mg) * s;
        *b = mb + (pb - mb) * s;

    } else if (t < 0.45f) {

        float s = (t - 0.20f) / 0.25f;

        *r = pr + (br - pr) * s;
        *g = pg + (bg2 - pg) * s;
        *b = pb + (bb - pb) * s;

    } else if (t < 0.70f) {

        float s = (t - 0.45f) / 0.25f;

        *r = br + (cr - br) * s;
        *g = bg2 + (cg - bg2) * s;
        *b = bb + (cb - bb) * s;

    } else {

        float s = (t - 0.70f) / 0.30f;

        *r = cr + (pir - cr) * s;
        *g = cg + (pig - cg) * s;
        *b = cb + (pib - cb) * s;
    }
}

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void render_orb_line(
    int y,
    Uint32 *pixels,
    int width,
    int height,
    float time,
    float audio)
{
    for (int x = 0; x < width; x++) {

        float u = (float)x / (float)width;
        float v = (float)y / (float)height;

        float px = u - 0.5f;
        float py = v - 0.5f;

        float radius = 0.285f;
        radius += audio * 0.018f;

        float r = sqrtf(px * px + py * py);

        float inside = 1.0f - clampf(
            (r - radius) / 0.012f,
            0.0f, 1.0f
        );

        float t = time;

        float qx = px / radius;
        float qy = py / radius;

        float qlen = sqrtf(
            qx * qx + qy * qy
        );

        float sphereZ = sqrtf(
            fmaxf(
                0.0f,
                1.0f - qlen * qlen
            )
        );

        float flow_x =
            qx + sinf(
                qy * 4.0f + t * 0.75f
            ) * 0.10f;

        float flow_y =
            qy + cosf(
                qx * 5.0f - t * 0.62f
            ) * 0.08f;

        float n1 = fbm(
            flow_x * 2.7f + t * 0.08f,
            flow_y * 2.7f - t * 0.05f
        );

        float n2 = fbm(
            flow_x * 5.0f - t * 0.12f,
            flow_y * 5.0f + t * 0.09f
        );

        float wave1 = sinf(
            qx * 4.0f +
            qy * 2.0f +
            t * 1.25f +
            n1 * 2.2f
        );

        float ribbon1 =
            expf(-fabsf(wave1) * 2.8f);

        float wave2 = sinf(
            qx * 7.0f -
            qy * 3.0f -
            t * 1.6f +
            n2 * 3.0f
        );

        float ribbon2 =
            expf(-fabsf(wave2) * 4.0f);

        float wave3 = sinf(
            qx * 12.0f +
            qy * 5.0f +
            t * 2.2f +
            n2 * 5.0f
        );

        float ribbon3 =
            expf(-fabsf(wave3) * 7.0f);

        float colorPos =
            qx * 0.38f +
            qy * 0.25f +
            n1 * 0.55f +
            t * 0.035f;

        float c1r, c1g, c1b;
        float c2r, c2g, c2b;
        float c3r, c3g, c3b;

        palette(colorPos,
            &c1r, &c1g, &c1b);

        palette(colorPos + 0.37f,
            &c2r, &c2g, &c2b);

        palette(colorPos + 0.67f,
            &c3r, &c3g, &c3b);

        float color_r = c1r;
        float color_g = c1g;
        float color_b = c1b;

        color_r += (c2r - color_r) *
            ribbon2 * 0.50f;

        color_g += (c2g - color_g) *
            ribbon2 * 0.50f;

        color_b += (c2b - color_b) *
            ribbon2 * 0.50f;

        color_r += (c3r - color_r) *
            ribbon3 * 0.20f;

        color_g += (c3g - color_g) *
            ribbon3 * 0.20f;

        color_b += (c3b - color_b) *
            ribbon3 * 0.20f;

        float intensity =
            ribbon1 * 0.95f +
            ribbon2 * 0.45f +
            ribbon3 * 0.18f;

        intensity *=
            0.75f + audio * 0.90f;

        float inv_len =
            1.0f / sqrtf(
                qx * qx +
                qy * qy +
                sphereZ * sphereZ +
                0.0001f
            );

        float nx = qx * inv_len;
        float ny = qy * inv_len;
        float nz = sphereZ * inv_len;

        float ldx = -0.35f;
        float ldy = -0.45f;
        float ldz = 1.0f;

        float ld_len = sqrtf(
            ldx * ldx +
            ldy * ldy +
            ldz * ldz
        );

        ldx /= ld_len;
        ldy /= ld_len;
        ldz /= ld_len;

        float diffuse =
            nx * ldx +
            ny * ldy +
            nz * ldz;

        if (diffuse < 0.0f)
            diffuse = 0.0f;

        color_r *= 0.72f + diffuse * 0.55f;
        color_g *= 0.72f + diffuse * 0.55f;
        color_b *= 0.72f + diffuse * 0.55f;

        float nz_clamped = nz;
        if (nz_clamped < 0.0f)
            nz_clamped = 0.0f;

        float fresnel =
            powf(
                1.0f - nz_clamped,
                3.5f
            );

        color_r += 0.25f * fresnel * 0.75f;
        color_g += 0.65f * fresnel * 0.75f;
        color_b += 1.00f * fresnel * 0.75f;

        color_r *= intensity;
        color_g *= intensity;
        color_b *= intensity;

        float shadow = 0.0f;
        {
            float sx = px;
            float sy = py - 0.05f;
            float sr = sqrtf(sx * sx + sy * sy);
            shadow = expf(
                -(sr * sr) * 8.0f
            ) * 0.90f;
            shadow *= 1.0f - inside;
        }

        float final_r =
            color_r * inside;

        float final_g =
            color_g * inside;

        float final_b =
            color_b * inside;

        float alpha = inside + shadow;

        float edge = clampf(
            (r - (radius - 0.035f)) /
                0.035f,
            0.0f, 1.0f
        );

        final_r += 0.25f * edge * 0.55f * inside;
        final_g += 0.55f * edge * 0.55f * inside;
        final_b += 1.00f * edge * 0.55f * inside;

        float centerGlow =
            expf(-r * r * 35.0f) * inside;

        final_r += 1.0f * centerGlow * 1.8f;
        final_g += 0.55f * centerGlow * 1.8f;
        final_b += 0.85f * centerGlow * 1.8f;

        float whiteCore =
            expf(-r * r * 80.0f) * inside;

        final_r += 1.0f * whiteCore * 1.5f;
        final_g += 0.90f * whiteCore * 1.5f;
        final_b += 0.98f * whiteCore * 1.5f;

        float innerGlow =
            expf(-r * r * 12.0f) * inside;

        final_r += 0.45f * innerGlow * 0.65f;
        final_g += 0.75f * innerGlow * 0.65f;
        final_b += 1.00f * innerGlow * 0.65f;

        final_r *= intensity;
        final_g *= intensity;
        final_b *= intensity;

        if (final_r < 0.0f) final_r = 0.0f;
        if (final_g < 0.0f) final_g = 0.0f;
        if (final_b < 0.0f) final_b = 0.0f;

        if (final_r > 1.0f) final_r = 1.0f;
        if (final_g > 1.0f) final_g = 1.0f;
        if (final_b > 1.0f) final_b = 1.0f;

        float brightness =
            (final_r + final_g + final_b) /
            3.0f;

        Uint8 a =
            (Uint8)(clampf(
                alpha * 255.0f, 0.0f, 255.0f
            ));

        if (brightness < 0.01f)
            a = 0;

        Uint8 ri =
            (Uint8)(final_r * 255.0f);
        Uint8 gi =
            (Uint8)(final_g * 255.0f);
        Uint8 bi =
            (Uint8)(final_b * 255.0f);

        pixels[y * width + x] =
            (a << 24) |
            (ri << 16) |
            (gi << 8) |
            bi;
    }
}

void orb_init(
    Orb *orb,
    SDL_Renderer *renderer)
{
    orb->texture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            ORB_RES,
            ORB_RES
        );

    SDL_SetTextureBlendMode(
        orb->texture,
        SDL_BLENDMODE_BLEND
    );

    orb->time = 0.0f;
    orb->audio = 0.0f;
    orb->dirty = true;
    orb->visible = true;

    memset(
        orb->pixels,
        0,
        sizeof(orb->pixels)
    );
}

void orb_free(Orb *orb)
{
    if (orb->texture) {
        SDL_DestroyTexture(orb->texture);
        orb->texture = NULL;
    }
}

void orb_update(Orb *orb, float dt)
{
    orb->time += dt;

    orb->audio =
        0.50f
        + 0.25f * sinf(orb->time * 2.1f)
        + 0.15f * sinf(orb->time * 5.7f)
        + 0.08f * sinf(orb->time * 11.0f);

    if (orb->audio < 0.0f)
        orb->audio = 0.0f;

    if (orb->audio > 1.0f)
        orb->audio = 1.0f;

    for (int y = 0; y < ORB_RES; y++) {

        render_orb_line(
            y,
            orb->pixels,
            ORB_RES,
            ORB_RES,
            orb->time,
            orb->audio
        );
    }

    orb->dirty = true;
}

void orb_draw(
    Orb *orb,
    SDL_Renderer *renderer,
    SDL_Rect dst)
{
    if (!orb->texture || !orb->visible)
        return;

    if (orb->dirty) {

        void *pixels;
        int pitch;

        SDL_LockTexture(
            orb->texture,
            NULL,
            &pixels,
            &pitch
        );

        memcpy(
            pixels,
            orb->pixels,
            ORB_RES * ORB_RES * 4
        );

        SDL_UnlockTexture(orb->texture);

        orb->dirty = false;
    }

    SDL_RenderCopy(
        renderer,
        orb->texture,
        NULL,
        &dst
    );
}
