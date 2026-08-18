
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

#include "ui.h"
#include "panel.h"
#include "button.h"
#include "slider.h"
#include "radio.h"
#include "checkbox.h"

static char *find_font(void)
{
    DIR *dir = opendir("font");

    if (!dir)
        return NULL;

    struct dirent *entry;

    while ((entry = readdir(dir))) {

        const char *name = entry->d_name;

        size_t len = strlen(name);

        if (
            len > 4 &&
            strcasecmp(
                name + len - 4,
                ".ttf"
            ) == 0
        ) {
            char *path =
                malloc(
                    strlen("font/") +
                    strlen(name) + 1
                );

            sprintf(
                path,
                "font/%s",
                name
            );

            closedir(dir);

            return path;
        }
    }

    closedir(dir);

    return NULL;
}

static void draw_background(
    SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(
        renderer,
        218,
        228,
        247,
        255
    );

    SDL_RenderClear(renderer);

    /*
        Crisp colored background shapes.

        They make the translucent material visible
        without requiring blur.
    */

    SDL_SetRenderDrawColor(
        renderer,
        120,
        160,
        235,
        255
    );

    SDL_Rect blue = {
        690, 70, 320, 190
    };

    SDL_RenderFillRect(
        renderer,
        &blue
    );

    SDL_SetRenderDrawColor(
        renderer,
        235,
        165,
        205,
        255
    );

    SDL_Rect pink = {
        730, 450, 280, 150
    };

    SDL_RenderFillRect(
        renderer,
        &pink
    );

    SDL_SetRenderDrawColor(
        renderer,
        125,
        215,
        190,
        255
    );

    SDL_Rect green = {
        530, 90, 150, 120
    };

    SDL_RenderFillRect(
        renderer,
        &green
    );
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {

        fprintf(
            stderr,
            "%s\n",
            SDL_GetError()
        );

        return 1;
    }

    if (TTF_Init() != 0) {

        fprintf(
            stderr,
            "%s\n",
            TTF_GetError()
        );

        SDL_Quit();

        return 1;
    }

    SDL_Window *window =
        SDL_CreateWindow(
            "SDL2 Liquid Glass UI",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1100,
            700,
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_RESIZABLE
        );

    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC
        );

    if (!window || !renderer)
        return 1;

    char *fontPath =
        find_font();

    if (!fontPath) {

        printf(
            "Put a .ttf file in ./font/\n"
        );

        return 1;
    }

    TTF_Font *font =
        TTF_OpenFont(
            fontPath,
            19
        );

    TTF_Font *titleFont =
        TTF_OpenFont(
            fontPath,
            36
        );

    free(fontPath);

    if (!font || !titleFont) {

        printf(
            "Could not load font\n"
        );

        return 1;
    }

    /*
        -------------------------------------------------
        UI OBJECTS
        -------------------------------------------------
    */

    UIPanel panel;

    panel_init(
        &panel,
        "Material controls"
    );

    UIButton button;

    button_init(
        &button,
        "Glass Button"
    );

    UISlider slider;

    slider_init(
        &slider,
        0.58f
    );

    UIRadio radios[3];

    radio_init(
        &radios[0],
        "Liquid Glass",
        true
    );

    radio_init(
        &radios[1],
        "Frosted",
        false
    );

    radio_init(
        &radios[2],
        "Solid",
        false
    );

    UICheckBox checkbox;

    checkbox_init(
        &checkbox,
        "Enable reflections",
        true
    );

    UIContext ui;

    bool running = true;

    Uint64 start =
        SDL_GetPerformanceCounter();

    while (running) {

        int width;
        int height;

        SDL_GetWindowSize(
            window,
            &width,
            &height
        );

        /*
            Recalculate UI scale.
        */

        ui_begin(
            &ui,
            width,
            height
        );

        /*
            ------------------------------------------------
            RESPONSIVE LAYOUT
            ------------------------------------------------

            Everything is positioned from the same
            layout coordinates.

            Notice that no widget is placed on top
            of another widget.
        */

        panel_layout(
            &panel,
            &ui,
            55,
            190,
            480,
            385
        );

        button_layout(
            &button,
            &ui,
            90,
            480,
            190,
            55
        );

        slider_layout(
            &slider,
            &ui,
            310,
            280,
            190
        );

        /*
            Radio group.
        */

        radio_layout(
            &radios[0],
            &ui,
            90,
            280
        );

        radio_layout(
            &radios[1],
            &ui,
            90,
            325
        );

        radio_layout(
            &radios[2],
            &ui,
            90,
            370
        );

        checkbox_layout(
            &checkbox,
            &ui,
            310,
            350
        );

        /*
            Events.
        */

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (
                event.type ==
                SDL_QUIT
            ) {
                running = false;
            }

            button_event(
                &button,
                &event
            );

            slider_event(
                &slider,
                &event
            );

            checkbox_event(
                &checkbox,
                &event
            );

            /*
                Radio group.

                Only one radio can be selected.
            */

            for (int i = 0; i < 3; i++) {

                if (
                    radio_event(
                        &radios[i],
                        &event
                    )
                ) {

                    for (int j = 0; j < 3; j++) {

                        if (j != i)
                            radios[j].selected =
                                false;
                    }
                }
            }

            if (
                event.type ==
                SDL_KEYDOWN &&
                event.key.keysym.sym ==
                SDLK_ESCAPE
            ) {
                running = false;
            }
        }

        if (button.clicked) {

            printf(
                "Button clicked\n"
            );
        }

        /*
            Time.
        */

        Uint64 now =
            SDL_GetPerformanceCounter();

        float time =
            (float)(
                (double)(
                    now - start
                ) /
                SDL_GetPerformanceFrequency()
            );

        /*
            ------------------------------------------------
            RENDER
            ------------------------------------------------
        */

        draw_background(
            renderer
        );

        /*
            Header.
        */

        ui_text(
            renderer,
            titleFont,
            "Liquid Glass",
            (int)(55 * ui.scale),
            (int)(60 * ui.scale),
            (UIColor){35, 43, 58, 255}
        );

        ui_text(
            renderer,
            font,
            "C / SDL2 scalable interface",
            (int)(58 * ui.scale),
            (int)(112 * ui.scale),
            (UIColor){82, 92, 110, 255}
        );

        /*
            Main panel.
        */

        panel_draw(
            &panel,
            &ui,
            renderer,
            font,
            time
        );

        /*
            Controls.
        */

        for (int i = 0; i < 3; i++) {

            radio_draw(
                &radios[i],
                &ui,
                renderer,
                font
            );
        }

        slider_draw(
            &slider,
            &ui,
            renderer,
            font,
            time
        );

        checkbox_draw(
            &checkbox,
            &ui,
            renderer,
            font,
            time
        );

        button_draw(
            &button,
            &ui,
            renderer,
            font,
            time
        );

        /*
            Right-side information panel.

            Its position is calculated from the window
            rather than hard-coded over the left panel.
        */

        float rightX =
            570;

        if (width < 800) {

            /*
                On narrow windows, move the secondary
                panel below the main panel instead of
                allowing overlap.
            */

            rightX = 55;
        }

        SDL_Rect info =
            ui_rect(
                &ui,
                rightX,
                width < 800 ? 600 : 190,
                450,
                190
            );

        ui_glass(
            renderer,
            info,
            (int)(24 * ui.scale),
            false,
            time
        );

        ui_text(
            renderer,
            font,
            "Optical material",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(22 * ui.scale),
            (UIColor){35, 43, 58, 255}
        );

        ui_text(
            renderer,
            font,
            "Crisp translucency",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(65 * ui.scale),
            (UIColor){82, 92, 110, 255}
        );

        ui_text(
            renderer,
            font,
            "Edge lighting",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(98 * ui.scale),
            (UIColor){82, 92, 110, 255}
        );

        ui_text(
            renderer,
            font,
            "Simulated lensing",
            info.x + (int)(220 * ui.scale),
            info.y + (int)(65 * ui.scale),
            (UIColor){82, 92, 110, 255}
        );

        ui_text(
            renderer,
            font,
            "Responsive layout",
            info.x + (int)(220 * ui.scale),
            info.y + (int)(98 * ui.scale),
            (UIColor){82, 92, 110, 255}
        );

        SDL_RenderPresent(
            renderer
        );
    }

    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);

    SDL_DestroyRenderer(
        renderer
    );

    SDL_DestroyWindow(
        window
    );

    TTF_Quit();
    SDL_Quit();

    return 0;
}
