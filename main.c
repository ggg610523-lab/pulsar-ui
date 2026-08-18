
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

#include "ui.h"
#include "panel.h"
#include "button.h"
#include "primaryButton.h"
#include "slider.h"
#include "radio.h"
#include "checkbox.h"
#include "searchBar.h"
#include "hamburger.h"
#include "sidebar.h"
#include "dialogBox.h"
#include "dropdownmenu.h"
#include "progressBar.h"
#include "tabs.h"
#include "menuBar.h"
#include "tooltips.h"
#include "notifications.h"

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
    SDL_Renderer *renderer,
    int width,
    int height,
    float time,
    bool dark)
{
    /*
        Soft vertical gradient base.
    */
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

    /*
        Animated gradient orbs.
        Soft radial blobs that drift slowly,
        creating a vibrant living background
        for the glass panels to refract.
    */

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

        /* Large blue-violet orb */
        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.55f +
                  sinf(t1) * 60.0f),
            (int)(height * 0.25f +
                  cosf(t1 * 0.7f) * 40.0f),
            (int)(width * 0.28f),
            (UIColor){110, 140, 235, 90},
            (UIColor){110, 140, 235, 0});

        /* Warm pink-magenta orb */
        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.72f +
                  cosf(t2) * 50.0f),
            (int)(height * 0.70f +
                  sinf(t2 * 0.8f) * 35.0f),
            (int)(width * 0.22f),
            (UIColor){230, 130, 195, 80},
            (UIColor){230, 130, 195, 0});

        /* Teal-green orb */
        ui_fill_radial_gradient(
            renderer,
            (int)(width * 0.38f +
                  sinf(t3 * 0.6f) * 45.0f),
            (int)(height * 0.60f +
                  cosf(t3) * 30.0f),
            (int)(width * 0.18f),
            (UIColor){100, 200, 185, 70},
            (UIColor){100, 200, 185, 0});

        /* Soft lavender accent */
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

    SDL_StartTextInput();

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

    SDL_SetHint(
        SDL_HINT_RENDER_SCALE_QUALITY,
        "1");

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

    UIPrimaryButton primaryButton;

    primary_button_init(
        &primaryButton,
        "OK"
    );

    UISlider slider;

    slider_init(
        &slider,
        0.58f
    );

    UIProgressBar progressbar;

    progress_bar_init(
        &progressbar,
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

    UITabs tabs;

    tabs_init(
        &tabs
    );

    tabs_add(
        &tabs,
        "General"
    );

    tabs_add(
        &tabs,
        "Appearance"
    );

    tabs_add(
        &tabs,
        "Advanced"
    );

    UISearchBar searchbar;

    searchbar_init(
        &searchbar
    );

    UIDropDown dropdown;

    dropdown_init(
        &dropdown
    );

    dropdown_add_item(
        &dropdown,
        "Option 1"
    );

    dropdown_add_item(
        &dropdown,
        "Option 2"
    );

    dropdown_add_item(
        &dropdown,
        "Option 3"
    );

    dropdown_add_item(
        &dropdown,
        "Option 4"
    );

    UIHamburger hamburger;

    hamburger_init(
        &hamburger
    );

    UIMenuBar menubar;

    menubar_init(
        &menubar
    );

    menubar_add_menu(
        &menubar,
        "File"
    );

    menubar_add_item(
        &menubar, 0, "New"
    );

    menubar_add_item(
        &menubar, 0, "Open"
    );

    menubar_add_item(
        &menubar, 0, "Save"
    );

    menubar_add_item(
        &menubar, 0, "Exit"
    );

    menubar_add_menu(
        &menubar,
        "Edit"
    );

    menubar_add_item(
        &menubar, 1, "Undo"
    );

    menubar_add_item(
        &menubar, 1, "Redo"
    );

    menubar_add_item(
        &menubar, 1, "Copy"
    );

    menubar_add_item(
        &menubar, 1, "Paste"
    );

    menubar_add_menu(
        &menubar,
        "Help"
    );

    menubar_add_item(
        &menubar, 2, "About"
    );

    menubar_add_item(
        &menubar, 2, "Documentation"
    );

    UISidebar sidebar;

    sidebar_init(
        &sidebar
    );

    DialogBox dialog;

    dialog_box_init(
        &dialog,
        font
    );

    UIContext ui;
    ui.dark = false;

    UITooltipManager tooltips;

    tooltip_manager_init(
        &tooltips
    );

    NotificationManager notifications;

    notif_manager_init(
        &notifications
    );

    UIButton notifButton;

    button_init(
        &notifButton,
        "Notify"
    );

    bool running = true;

    Uint64 start =
        SDL_GetPerformanceCounter();

    SDL_Texture *rt = NULL;
    int rt_w = 0;
    int rt_h = 0;

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

        tabs_layout(
            &tabs,
            &ui,
            570,
            410,
            450
        );

        button_layout(
            &button,
            &ui,
            90,
            480,
            190,
            55
        );

        primary_button_layout(
            &primaryButton,
            &ui,
            300,
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

        progress_bar_layout(
            &progressbar,
            &ui,
            570,
            490,
            450,
            12
        );

        button_layout(
            &notifButton,
            &ui,
            570,
            520,
            120,
            45
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

        searchbar_layout(
            &searchbar,
            &ui,
            55,
            590,
            480,
            45
        );

        dropdown_layout(
            &dropdown,
            &ui,
            310,
            420,
            190
        );

        hamburger_layout(
            &hamburger,
            &ui,
            15,
            15,
            40
        );

        menubar_layout(
            &menubar,
            &ui,
            570,
            15,
            450
        );

        tooltip_manager_init(&tooltips);

        tooltip_add(
            &tooltips,
            button.rect,
            "Click to open dialog"
        );

        tooltip_add(
            &tooltips,
            primaryButton.rect,
            "Primary action"
        );

        tooltip_add(
            &tooltips,
            slider.rect,
            "Adjust material intensity"
        );

        tooltip_add(
            &tooltips,
            checkbox.rect,
            "Toggle reflections"
        );

        tooltip_add(
            &tooltips,
            hamburger.rect,
            "Open menu"
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

            primary_button_event(
                &primaryButton,
                &event
            );

            slider_event(
                &slider,
                &event
            );

            progress_bar_set_value(
                &progressbar,
                slider.value
            );

            checkbox_event(
                &checkbox,
                &event
            );

            tabs_event(
                &tabs,
                &event
            );

            dropdown_event(
                &dropdown,
                &event
            );

            searchbar_event(
                &searchbar,
                &event
            );

            hamburger_event(
                &hamburger,
                &event
            );

            menubar_event(
                &menubar,
                &event
            );

            if (hamburger.clicked) {
                sidebar.open = !sidebar.open;
            }

            sidebar_event(
                &sidebar,
                &event
            );

            hamburger.open = sidebar.open;

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

            if (
                event.type ==
                SDL_KEYDOWN &&
                event.key.keysym.sym ==
                SDLK_d
            ) {
                ui.dark = !ui.dark;
            }

            dialog_box_event(
                &dialog,
                &event
            );

            button_event(
                &notifButton,
                &event
            );
        }

        if (button.clicked) {
            dialog_box_open(&dialog);
        }

        if (primaryButton.clicked) {

            printf(
                "Primary button clicked\n"
            );
        }

        if (notifButton.clicked) {

            notif_push(
                &notifications,
                "Hello from notification!"
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

        static float prev_time = 0.0f;
        float dt = time - prev_time;
        prev_time = time;

        tooltip_update(
            &tooltips,
            NULL,
            dt
        );

        notif_update(
            &notifications,
            dt
        );

        /*
            ------------------------------------------------
            RENDER (supersampled at 2x)
            ------------------------------------------------
        */

        if (width != rt_w || height != rt_h) {

            if (rt)
                SDL_DestroyTexture(rt);

            rt_w = width;
            rt_h = height;

            rt =
                SDL_CreateTexture(
                    renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_TARGET,
                    width * 2,
                    height * 2);

            SDL_SetTextureBlendMode(
                rt, SDL_BLENDMODE_BLEND);
        }

        SDL_SetRenderTarget(renderer, rt);
        SDL_RenderSetScale(renderer, 2.0f, 2.0f);

        ui_text_cache_clear();

        draw_background(
            renderer,
            width,
            height,
            time,
            ui.dark
        );

        /*
            Tabs.
        */

        tabs_draw(
            &tabs,
            &ui,
            renderer,
            font,
            dt
        );

        /*
            Main panel.
        */

        panel_draw(
            &panel,
            &ui,
            renderer,
            font
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
            font
        );

        progress_bar_draw(
            &progressbar,
            &ui,
            renderer,
            dt
        );

        checkbox_draw(
            &checkbox,
            &ui,
            renderer,
            font
        );

        button_draw(
            &button,
            &ui,
            renderer,
            font
        );

        primary_button_draw(
            &primaryButton,
            &ui,
            renderer,
            font
        );

        dropdown_draw(
            &dropdown,
            &ui,
            renderer,
            font,
            dt
        );

        searchbar_draw(
            &searchbar,
            &ui,
            renderer,
            font,
            dt
        );

        hamburger_draw(
            &hamburger,
            &ui,
            renderer
        );

        /*
            Sidebar.
        */

        sidebar_layout(
            &sidebar,
            &ui,
            0,
            0,
            260,
            height / ui.scale
        );

        sidebar_draw(
            &sidebar,
            &ui,
            renderer,
            font,
            dt
        );

        /*
            Right-side information panel.
        */

        float rightX =
            570;

        if (width < 800) {

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
            ui.dark
        );

        /*
            Tooltips (drawn on top of everything).
        */

        tooltip_draw(
            &tooltips,
            &ui,
            renderer,
            font
        );

        button_draw(
            &notifButton,
            &ui,
            renderer,
            font
        );

        /*
            Menu bar.
        */

        menubar_draw(
            &menubar,
            &ui,
            renderer,
            font,
            dt
        );

        /*
            Downscale 2x texture to window.
        */
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderSetScale(renderer, 1.0f, 1.0f);

        SDL_Rect dst = { 0, 0, width, height };
        SDL_RenderCopy(
            renderer, rt, NULL, &dst);

        /*
            Text at native resolution.
        */

        float titleX =
            55.0f +
            ui_ease_out_cubic(sidebar.anim) *
            280.0f;

        ui_text_shadow(
            renderer,
            titleFont,
            "Liquid Glass",
            (int)(titleX * ui.scale),
            (int)(60 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){25, 32, 50, 255},
                (UIColor){220, 225, 240, 255}),
            ui_theme(ui.dark,
                (UIColor){255, 255, 255, 45},
                (UIColor){0, 0, 0, 60}),
            1);

        ui_text_shadow(
            renderer,
            font,
            "C / SDL2 scalable interface",
            (int)((titleX + 3) * ui.scale),
            (int)(112 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255}),
            ui_theme(ui.dark,
                (UIColor){255, 255, 255, 30},
                (UIColor){0, 0, 0, 40}),
            1);

        ui_text_shadow(
            renderer,
            font,
            "Optical material",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(22 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){25, 32, 50, 255},
                (UIColor){220, 225, 240, 255}),
            ui_theme(ui.dark,
                (UIColor){255, 255, 255, 35},
                (UIColor){0, 0, 0, 50}),
            1);

        ui_text(
            renderer,
            font,
            "Crisp translucency",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(65 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            renderer,
            font,
            "Edge lighting",
            info.x + (int)(25 * ui.scale),
            info.y + (int)(98 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            renderer,
            font,
            "Simulated lensing",
            info.x + (int)(220 * ui.scale),
            info.y + (int)(65 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            renderer,
            font,
            "Responsive layout",
            info.x + (int)(220 * ui.scale),
            info.y + (int)(98 * ui.scale),
            ui_theme(ui.dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        /*
            Notifications at native resolution.
        */

        notif_draw(
            &notifications,
            &ui,
            renderer,
            font
        );

        SDL_RenderPresent(
            renderer
        );

        dialog_box_draw(
            &dialog,
            ui.dark,
            dt
        );
    }

    dialog_box_close(&dialog);

    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);

    ui_text_cache_shutdown();

    if (rt)
        SDL_DestroyTexture(rt);

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
