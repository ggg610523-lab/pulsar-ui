#include "panel.h"

void panel_init(
    UIPanel *panel,
    const char *title)
{
    panel->title = title;
}

void panel_layout(
    UIPanel *panel,
    UIContext *ui,
    float x,
    float y,
    float w,
    float h)
{
    panel->rect =
        ui_rect(
            ui,
            x,
            y,
            w,
            h
        );
}

void panel_draw(
    UIPanel *panel,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font)
{
    int radius =
        (int)roundf(
            28.0f * ui->scale
        );

    ui_glass(
        renderer,
        panel->rect,
        radius,
        false,
        ui->dark
    );

    int padding =
        (int)roundf(
            25.0f * ui->scale
        );

    ui_text(
        renderer,
        font,
        panel->title,
        panel->rect.x + padding,
        panel->rect.y + padding,
        ui_theme(ui->dark,
            (UIColor){35, 45, 62, 255},
            (UIColor){180, 190, 210, 255}
        ));
}
