#include "background.h"
#include <math.h>

void draw_background_base(
    SDL_Renderer *renderer,
    int width,
    int height,
    bool dark)
{
    for (int y = 0; y < height; ++y) {

        float t =
            (float)y /
            (float)(height - 1);

        Uint8 r, g, b;

        if (dark) {

            r = (Uint8)(18 + t * 10.0f);
            g = (Uint8)(20 + t * 8.0f);
            b = (Uint8)(35 - t * 5.0f);

        } else {

            r = (Uint8)(205 + t * 25.0f);
            g = (Uint8)(215 + t * 18.0f);
            b = (Uint8)(242 - t * 5.0f);
        }

        SDL_SetRenderDrawColor(
            renderer, r, g, b, 255);

        SDL_RenderDrawLine(
            renderer,
            0, y,
            width - 1, y);
    }
}

void draw_background_orbs(
    SDL_Renderer *renderer,
    int width,
    int height,
    float time,
    bool dark)
{
    float t1 = time * 0.15f;
    float t2 = time * 0.12f + 2.0f;
    float t3 = time * 0.18f + 4.0f;

    if (dark) {

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.55f +
                  sinf(t1) * 60.0f),
            (int)(height * 0.25f +
                  cosf(t1 * 0.7f) * 40.0f),
            (int)(width * 0.28f),
            (UIColor){50, 60, 140, 60},
            (UIColor){50, 60, 140, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.72f +
                  cosf(t2) * 50.0f),
            (int)(height * 0.70f +
                  sinf(t2 * 0.8f) * 35.0f),
            (int)(width * 0.22f),
            (UIColor){130, 50, 100, 50},
            (UIColor){130, 50, 100, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.38f +
                  sinf(t3 * 0.6f) * 45.0f),
            (int)(height * 0.60f +
                  cosf(t3) * 30.0f),
            (int)(width * 0.18f),
            (UIColor){40, 110, 95, 45},
            (UIColor){40, 110, 95, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.85f +
                  sinf(t1 * 0.9f) * 35.0f),
            (int)(height * 0.15f +
                  cosf(t2 * 0.5f) * 25.0f),
            (int)(width * 0.15f),
            (UIColor){80, 60, 130, 35},
            (UIColor){80, 60, 130, 0});

    } else {

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.55f +
                  sinf(t1) * 60.0f),
            (int)(height * 0.25f +
                  cosf(t1 * 0.7f) * 40.0f),
            (int)(width * 0.28f),
            (UIColor){110, 140, 235, 90},
            (UIColor){110, 140, 235, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.72f +
                  cosf(t2) * 50.0f),
            (int)(height * 0.70f +
                  sinf(t2 * 0.8f) * 35.0f),
            (int)(width * 0.22f),
            (UIColor){230, 130, 195, 80},
            (UIColor){230, 130, 195, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.38f +
                  sinf(t3 * 0.6f) * 45.0f),
            (int)(height * 0.60f +
                  cosf(t3) * 30.0f),
            (int)(width * 0.18f),
            (UIColor){100, 200, 185, 70},
            (UIColor){100, 200, 185, 0});

        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.85f +
                  sinf(t1 * 0.9f) * 35.0f),
            (int)(height * 0.15f +
                  cosf(t2 * 0.5f) * 25.0f),
            (int)(width * 0.15f),
            (UIColor){170, 140, 220, 55},
            (UIColor){170, 140, 220, 0});
    }
}

void draw_background(
    SDL_Renderer *renderer,
    int width,
    int height,
    float time,
    bool dark)
{
    draw_background_base(
        renderer, width, height, dark);

    draw_background_orbs(
        renderer, width, height,
        time, dark);
}
