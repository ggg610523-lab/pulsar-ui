#include "slider.h"

static float clamp(
    float value,
    float min,
    float max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

void slider_init(
    UISlider *slider,
    float value)
{
    slider->value =
        clamp(value, 0.0f, 1.0f);

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
            40
        );
}

void slider_event(
    UISlider *slider,
    SDL_Event *event)
{
    if (
        event->type ==
        SDL_MOUSEBUTTONDOWN &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {

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

    if (
        event->type ==
        SDL_MOUSEBUTTONUP &&
        event->button.button ==
        SDL_BUTTON_LEFT
    ) {
        slider->dragging = false;
    }

    if (
        slider->dragging &&
        event->type ==
        SDL_MOUSEMOTION
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
    TTF_Font *font)
{
    int labelOffset =
        (int)roundf(
            28.0f * ui->scale
        );

    ui_text(
        renderer,
        font,
        "Material intensity",
        slider->rect.x,
        slider->rect.y - labelOffset,
        ui_theme(ui->dark,
            (UIColor){55, 65, 85, 255},
            (UIColor){175, 185, 205, 255})
    );

    /*
        Track.
    */
    int trackHeight =
        (int)roundf(8.0f * ui->scale);

    if (trackHeight < 4)
        trackHeight = 4;

    SDL_Rect track = {
        slider->rect.x,
        slider->rect.y +
            slider->rect.h / 2 -
            trackHeight / 2,
        slider->rect.w,
        trackHeight
    };

    ui_fill_rounded_rect(
        renderer,
        track,
        trackHeight / 2,
        ui_theme(ui->dark,
            (UIColor){205, 215, 230, 160},
            (UIColor){35, 42, 58, 160}));

    /*
        Active section.
    */
    int activeWidth =
        (int)roundf(
            slider->value *
            (float)track.w);

    if (activeWidth > 0) {

        if (activeWidth < trackHeight)
            activeWidth = trackHeight;

        if (activeWidth > track.w)
            activeWidth = track.w;

        SDL_Rect active = {
            track.x,
            track.y,
            activeWidth,
            track.h
        };

        ui_fill_rounded_rect_gradient(
            renderer,
            active,
            activeWidth < track.h
                ? activeWidth / 2
                : trackHeight / 2,
            (UIColor){
                75, 125, 245, 230},
            (UIColor){
                50, 95, 220, 230});
    }

    /*
        Knob.
    */
    int knobSize =
        (int)roundf(
            28.0f * ui->scale
        );

    if (knobSize < 16)
        knobSize = 16;

    int knobX =
        slider->rect.x +
        (int)roundf(
            slider->value *
            slider->rect.w
        );

    /*
        Clamp knob so it cannot extend outside
        its allocated control.
    */
    int half =
        knobSize / 2;

    int minX =
        slider->rect.x + half;

    int maxX =
        slider->rect.x +
        slider->rect.w -
        half;

    if (knobX < minX)
        knobX = minX;

    if (knobX > maxX)
        knobX = maxX;

    /*
        Knob shadow.
    */
    ui_fill_circle(
        renderer,
        knobX,
        slider->rect.y +
            slider->rect.h / 2 + 2,
        half,
        ui_theme(ui->dark,
            (UIColor){15, 20, 40, 25},
            (UIColor){0, 0, 0, 45}));

    /*
        Knob.
    */
    SDL_Rect knob = {
        knobX - half,
        slider->rect.y +
            slider->rect.h / 2 -
            half,
        knobSize,
        knobSize
    };

    ui_glass(
        renderer,
        knob,
        knobSize / 2,
        slider->dragging,
        ui->dark
    );

    /*
        Knob inner highlight.
    */
    int innerR = half - 3;

    if (innerR > 2) {

        ui_fill_circle(
            renderer,
            knobX,
            slider->rect.y +
                slider->rect.h / 2 - 1,
            innerR,
            (UIColor){
                255, 255, 255,
                ui->dark ? 15 : 30});
    }
}
