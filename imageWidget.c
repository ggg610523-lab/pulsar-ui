#include "imageWidget.h"
#include <string.h>

void image_widget_init(
    ImageWidget *iw,
    SDL_Renderer *renderer,
    const char *path)
{
    iw->loaded = false;
    iw->is_gif = false;

    size_t len = strlen(path);

    if (len > 4 &&
        strcasecmp(
            path + len - 4, ".gif") == 0) {

        if (gif_load(
                &iw->gif,
                renderer,
                path)) {

            iw->is_gif = true;
            iw->loaded = true;
            return;
        }
    }

    if (image_load(
            &iw->image,
            renderer,
            path)) {

        iw->is_gif = false;
        iw->loaded = true;
    }
}

void image_widget_free(ImageWidget *iw)
{
    if (iw->is_gif)
        gif_free(&iw->gif);
    else
        image_free(&iw->image);

    iw->loaded = false;
}

void image_widget_update(
    ImageWidget *iw,
    float dt)
{
    if (!iw->loaded)
        return;

    if (iw->is_gif)
        gif_update(&iw->gif, dt);
}

void image_widget_draw(
    ImageWidget *iw,
    SDL_Renderer *renderer,
    int x,
    int y,
    int w,
    int h)
{
    if (!iw->loaded)
        return;

    SDL_Rect dst = {x, y, w, h};

    if (iw->is_gif)
        gif_draw(&iw->gif, renderer, dst);
    else
        image_draw(
            &iw->image, renderer, dst);
}
