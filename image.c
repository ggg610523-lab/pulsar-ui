#include "image.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   Static image loading via SDL2_image
   ============================================================ */

bool image_load(
    Image *img,
    SDL_Renderer *renderer,
    const char *path)
{
    img->texture = NULL;
    img->w = 0;
    img->h = 0;

    SDL_Surface *surface =
        IMG_Load(path);

    if (!surface)
        return false;

    img->texture =
        SDL_CreateTextureFromSurface(
            renderer, surface);

    if (!img->texture) {
        SDL_FreeSurface(surface);
        return false;
    }

    img->w = surface->w;
    img->h = surface->h;

    SDL_FreeSurface(surface);

    return true;
}

void image_free(Image *img)
{
    if (img->texture) {
        SDL_DestroyTexture(img->texture);
        img->texture = NULL;
    }

    img->w = 0;
    img->h = 0;
}

void image_draw(
    Image *img,
    SDL_Renderer *renderer,
    SDL_Rect dst)
{
    if (!img->texture)
        return;

    SDL_RenderCopy(
        renderer,
        img->texture,
        NULL, &dst);
}

/* ============================================================
   GIF decoder
   ============================================================ */

typedef struct {
    const unsigned char *data;
    int size;
    int pos;
} GifReader;

static int gif_read_byte(GifReader *r)
{
    if (r->pos >= r->size)
        return -1;
    return r->data[r->pos++];
}

static int gif_read_short(GifReader *r)
{
    int lo = gif_read_byte(r);
    int hi = gif_read_byte(r);
    if (lo < 0 || hi < 0)
        return -1;
    return lo | (hi << 8);
}

static bool gif_read_bytes(
    GifReader *r,
    unsigned char *buf,
    int count)
{
    if (r->pos + count > r->size)
        return false;
    memcpy(buf, r->data + r->pos, count);
    r->pos += count;
    return true;
}

static bool gif_decode_lzw(
    GifReader *r,
    unsigned char *output,
    int output_size,
    int min_code_size)
{
    int clear_code = 1 << min_code_size;
    int eoi_code = clear_code + 1;
    int next_code = eoi_code + 1;
    int cur_bits = min_code_size + 1;

    int prefix[4096];
    unsigned char suffix[4096];
    int length[4096];

    for (int i = 0; i < clear_code; i++) {
        prefix[i] = -1;
        suffix[i] = (unsigned char)i;
        length[i] = 1;
    }

    int bit_buf = 0;
    int bits_left = 0;
    int prev = -1;
    int out_pos = 0;

    unsigned char stack[4096];
    int sp = 0;

    while (out_pos < output_size) {

        while (bits_left < cur_bits) {
            int b = gif_read_byte(r);
            if (b < 0)
                return out_pos > 0;
            bit_buf |= b << bits_left;
            bits_left += 8;
        }

        int code =
            bit_buf & ((1 << cur_bits) - 1);

        bit_buf >>= cur_bits;
        bits_left -= cur_bits;

        if (code == clear_code) {
            cur_bits = min_code_size + 1;
            next_code = eoi_code + 1;
            for (int i = 0;
                 i < clear_code; i++) {
                prefix[i] = -1;
                suffix[i] = (unsigned char)i;
                length[i] = 1;
            }
            prev = -1;
            continue;
        }

        if (code == eoi_code)
            break;

        int c;
        bool is_new = (code >= next_code);

        if (is_new) {
            if (prev < 0)
                break;
            c = prev;
            sp = 0;
            while (c >= 0) {
                stack[sp++] = suffix[c];
                c = prefix[c];
            }
            if (sp >= 4096)
                return false;
            output[out_pos++] = stack[sp - 1];
        } else {
            c = code;
            sp = 0;
            while (c >= 0) {
                stack[sp++] = suffix[c];
                c = prefix[c];
            }
            for (int i = sp - 1;
                 i >= 0; i--) {
                if (out_pos >= output_size)
                    return true;
                output[out_pos++] = stack[i];
            }
        }

        if (prev >= 0 &&
            next_code < 4096) {
            prefix[next_code] = prev;
            suffix[next_code] =
                stack[sp - 1];
            length[next_code] =
                length[prev] + 1;

            if (next_code ==
                (1 << cur_bits))
                cur_bits++;

            next_code++;
        }

        prev = is_new
            ? (next_code - 1)
            : code;
    }

    return out_pos > 0;
}

bool gif_load(
    Gif *gif,
    SDL_Renderer *renderer,
    const char *path)
{
    gif->frame_count = 0;
    gif->current_frame = 0;
    gif->elapsed = 0.0f;
    gif->playing = true;
    gif->loop = true;
    gif->w = 0;
    gif->h = 0;

    for (int i = 0; i < GIF_MAX_FRAMES; i++)
        gif->frames[i] = NULL;

    FILE *f = fopen(path, "rb");
    if (!f)
        return false;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize < 13) {
        fclose(f);
        return false;
    }

    unsigned char *buf = malloc(fsize);
    if (!buf) {
        fclose(f);
        return false;
    }

    if ((int)fread(buf, 1, fsize, f) !=
        (int)fsize) {
        free(buf);
        fclose(f);
        return false;
    }
    fclose(f);

    GifReader reader = {buf, (int)fsize, 0};

    if (memcmp(buf, "GIF87a", 6) != 0 &&
        memcmp(buf, "GIF89a", 6) != 0) {
        free(buf);
        return false;
    }

    reader.pos = 6;

    int sw = gif_read_short(&reader);
    int sh = gif_read_short(&reader);

    if (sw <= 0 || sh <= 0) {
        free(buf);
        return false;
    }

    gif->w = sw;
    gif->h = sh;

    unsigned char gct_buf[768];
    int gct_count = 0;

    unsigned char packed =
        buf[10];

    bool has_gct =
        (packed >> 7) & 1;

    int gct_bits =
        (packed >> 4) & 7;

    int bg_idx = buf[13];

    if (bg_idx < 0) bg_idx = 0;

    reader.pos = 13;

    if (has_gct) {
        gct_count = 1 << (gct_bits + 1);
        gif_read_bytes(
            &reader, gct_buf,
            gct_count * 3);
    }

    (void)bg_idx;

    unsigned char *canvas =
        calloc(sw * sh * 4, 1);

    if (!canvas) {
        free(buf);
        return false;
    }

    unsigned char *frame_data =
        malloc(sw * sh);

    if (!frame_data) {
        free(canvas);
        free(buf);
        return false;
    }

    while (gif->frame_count <
           GIF_MAX_FRAMES) {

        int b = gif_read_byte(&reader);

        if (b < 0)
            break;

        if (b == 0x21) {

            int sub = gif_read_byte(&reader);

            if (sub < 0)
                break;

            if (sub == 0xF9) {

                int block_size =
                    gif_read_byte(&reader);

                if (block_size != 4)
                    break;

                unsigned char disp_buf[4];
                gif_read_bytes(
                    &reader, disp_buf, 4);

                int flags = disp_buf[0];
                int delay =
                    disp_buf[1] |
                    (disp_buf[2] << 8);

                if (delay == 0)
                    delay = 10;

                int trans_idx = -1;
                if (flags & 1)
                    trans_idx = disp_buf[3];

                int dispose =
                    (flags >> 2) & 7;

                int block_term =
                    gif_read_byte(&reader);

                if (block_term != 0)
                    break;

                gif->delays[
                    gif->frame_count] =
                    delay;

                (void)trans_idx;
                (void)dispose;

            } else if (sub == 0xFF) {

                int block_size =
                    gif_read_byte(&reader);

                if (block_size > 0) {
                    reader.pos += block_size;
                }

                while (1) {
                    int sz =
                        gif_read_byte(&reader);

                    if (sz <= 0)
                        break;

                    reader.pos += sz;
                }

            } else {

                while (1) {
                    int sz =
                        gif_read_byte(&reader);

                    if (sz <= 0)
                        break;

                    reader.pos += sz;
                }
            }

        } else if (b == 0x2C) {

            int left =
                gif_read_short(&reader);
            int top =
                gif_read_short(&reader);
            int fw =
                gif_read_byte(&reader) |
                (gif_read_byte(&reader)
                 << 8);
            int fh =
                gif_read_byte(&reader) |
                (gif_read_byte(&reader)
                 << 8);

            if (fw <= 0) fw = sw;
            if (fh <= 0) fh = sh;

            int img_packed =
                gif_read_byte(&reader);

            bool has_lct =
                (img_packed >> 7) & 1;

            int lct_bits =
                (img_packed >> 4) & 7;

            int lct_count = 0;
            unsigned char lct_buf[768];

            if (has_lct) {
                lct_count =
                    1 << (lct_bits + 1);
                gif_read_bytes(
                    &reader, lct_buf,
                    lct_count * 3);
            }

            unsigned char *ct = gct_buf;
            int ct_count = gct_count;

            if (has_lct) {
                ct = lct_buf;
                ct_count = lct_count;
            }

            if (ct_count == 0) {
                break;
            }

            int min_code =
                gif_read_byte(&reader);

            if (min_code < 2 ||
                min_code > 11) {
                break;
            }

            memset(frame_data, 0,
                   sw * sh);

            bool ok = gif_decode_lzw(
                &reader,
                frame_data,
                sw * sh,
                min_code);

            if (!ok)
                break;

            for (int y = 0;
                 y < fh; y++) {

                for (int x = 0;
                     x < fw; x++) {

                    int dx = left + x;
                    int dy = top + y;

                    if (dx >= sw ||
                        dy >= sh)
                        continue;

                    int si =
                        y * fw + x;

                    unsigned char ci =
                        frame_data[si];

                    if (ci >= ct_count)
                        continue;

                    int di =
                        (dy * sw + dx) * 4;

                    canvas[di + 0] =
                        ct[ci * 3 + 2];

                    canvas[di + 1] =
                        ct[ci * 3 + 1];

                    canvas[di + 2] =
                        ct[ci * 3 + 0];

                    canvas[di + 3] = 255;
                }
            }

            SDL_Texture *tex =
                SDL_CreateTexture(
                    renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    sw, sh);

            if (tex) {

                void *pixels;
                int pitch;

                SDL_LockTexture(
                    tex, NULL,
                    &pixels, &pitch);

                memcpy(
                    pixels,
                    canvas,
                    sw * sh * 4);

                SDL_UnlockTexture(tex);

                gif->frames[
                    gif->frame_count] = tex;
            }

            gif->frame_count++;

        } else if (b == 0x3B) {

            break;
        }
    }

    free(frame_data);
    free(canvas);
    free(buf);

    return gif->frame_count > 0;
}

void gif_free(Gif *gif)
{
    for (int i = 0;
         i < gif->frame_count; i++) {

        if (gif->frames[i])
            SDL_DestroyTexture(
                gif->frames[i]);
    }

    gif->frame_count = 0;
    gif->current_frame = 0;
}

void gif_update(Gif *gif, float dt)
{
    if (!gif->playing ||
        gif->frame_count <= 1)
        return;

    gif->elapsed += dt * 1000.0f;

    float delay_ms =
        (float)gif->delays[
            gif->current_frame] * 10.0f;

    if (gif->elapsed >= delay_ms) {

        gif->elapsed -= delay_ms;

        gif->current_frame++;

        if (gif->current_frame >=
            gif->frame_count) {

            if (gif->loop)
                gif->current_frame = 0;
            else {
                gif->current_frame =
                    gif->frame_count - 1;

                gif->playing = false;
            }
        }
    }
}

void gif_draw(
    Gif *gif,
    SDL_Renderer *renderer,
    SDL_Rect dst)
{
    if (gif->frame_count == 0)
        return;

    SDL_Texture *tex =
        gif->frames[gif->current_frame];

    if (!tex)
        return;

    SDL_RenderCopy(
        renderer, tex, NULL, &dst);
}

void gif_reset(Gif *gif)
{
    gif->current_frame = 0;
    gif->elapsed = 0.0f;
    gif->playing = true;
}
