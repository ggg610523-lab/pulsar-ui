#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define GIF_MAX_FRAMES 256

typedef struct {
    SDL_Texture *texture;
    int w;
    int h;
} Image;

typedef struct {
    SDL_Texture *frames[GIF_MAX_FRAMES];
    int delays[GIF_MAX_FRAMES];
    int w;
    int h;
    int frame_count;
    int current_frame;
    float elapsed;
    bool playing;
    bool loop;
} Gif;

bool image_load(
    Image *img,
    SDL_Renderer *renderer,
    const char *path
);

void image_free(
    Image *img
);

void image_draw(
    Image *img,
    SDL_Renderer *renderer,
    SDL_Rect dst
);

bool gif_load(
    Gif *gif,
    SDL_Renderer *renderer,
    const char *path
);

void gif_free(
    Gif *gif
);

void gif_update(
    Gif *gif,
    float dt
);

void gif_draw(
    Gif *gif,
    SDL_Renderer *renderer,
    SDL_Rect dst
);

void gif_reset(
    Gif *gif
);

#endif /* IMAGE_H */
