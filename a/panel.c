
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
    TTF_Font *font,
    float time)
{
    ui_glass(
        renderer,
        panel->rect,
        (int)(24 * ui->scale),
        false,
        time
    );

    ui_text(
        renderer,
        font,
        panel->title,
        panel->rect.x + (int)(25 * ui->scale),
        panel->rect.y + (int)(22 * ui->scale),
        (UIColor){35, 45, 65, 255}
    );
}
