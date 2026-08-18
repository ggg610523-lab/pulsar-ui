
#include "slider.h"

static float clamp(
    float v,
    float min,
    float max)
{
    if (v < min) return min;
    if (v > max) return max;

    return v;
}

void slider_init(
    UISlider *slider,
    float value)
{
    slider->value = clamp(
        value,
        0.0f,
        1.0f
    );

    slider->dragging = false;
}

void slider_layout(
    UISlider *slider,
    UIContext *ui,
    float x,
    float y,
    float w)
{
    slider->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            32
        );
}

void slider_event(
    UISlider *slider,
    SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT) {

        if (
            ui_point_in_rect(
                event->button.x,
                event->button.y,
                slider->rect
            )
        ) {
            slider->dragging = true;
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP &&
        event->button.button == SDL_BUTTON_LEFT) {

        slider->dragging = false;
    }

    if (
        slider->dragging &&
        event->type == SDL_MOUSEMOTION
    ) {
        float value =
            (float)(
                event->motion.x -
                slider->rect.x
            ) /
            (float)slider->rect.w;

        slider->value =
            clamp(
                value,
                0.0f,
                1.0f
            );
    }
}

void slider_draw(
    UISlider *slider,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font,
    float time)
{
    int trackY =
        slider->rect.y +
        slider->rect.h / 2;

    /*
        Label.
    */

    ui_text(
        renderer,
        font,
        "Material intensity",
        slider->rect.x,
        slider->rect.y - (int)(27 * ui->scale),
        (UIColor){82, 92, 110, 255}
    );

    /*
        Track.
    */

    SDL_Rect track = {
        slider->rect.x,
        trackY - 3,
        slider->rect.w,
        6
    };

    ui_glass(
        renderer,
        track,
        3,
        false,
        time
    );

    /*
        Filled section.
    */

    SDL_Rect active = track;

    active.w =
        (int)(
            slider->value *
            track.w
        );

    SDL_SetRenderDrawColor(
        renderer,
        65,
        120,
        245,
        190
    );

    SDL_RenderFillRect(
        renderer,
        &active
    );

    /*
        Knob.

        It stays inside the track and therefore
        cannot overlap another UI element.
    */

    int knobX =
        slider->rect.x +
        (int)(
            slider->value *
            slider->rect.w
        );

    SDL_Rect knob = {
        knobX - 11,
        trackY - 14,
        22,
        28
    };

    ui_glass(
        renderer,
        knob,
        11,
        slider->dragging,
        time
    );
}
