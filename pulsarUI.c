#include "pulsarUI.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

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

/* -------------------------------------------------- */
/*  Internal: widget registry                         */
/* -------------------------------------------------- */

static void add_widget(
    PulsarUI *pui,
    void *data,
    PulsarEventFn on_event,
    PulsarDrawFn on_draw,
    PulsarLayoutFn on_layout,
    float x, float y,
    float w, float h)
{
    if (pui->widget_count >=
        pui->widget_cap) {

        pui->widget_cap =
            pui->widget_cap
            ? pui->widget_cap * 2
            : 16;

        pui->widgets =
            realloc(
                pui->widgets,
                sizeof(PulsarWidget) *
                pui->widget_cap
            );
    }

    PulsarWidget *wd =
        &pui->widgets[pui->widget_count++];

    wd->data = data;
    wd->on_event = on_event;
    wd->on_draw = on_draw;
    wd->on_layout = on_layout;
    wd->x = x;
    wd->y = y;
    wd->w = w;
    wd->h = h;
}

/* -------------------------------------------------- */
/*  Event wrappers                                    */
/* -------------------------------------------------- */

static void we_button(
    void *d, SDL_Event *e)
{
    button_event(d, e);
}

static void we_primary(
    void *d, SDL_Event *e)
{
    primary_button_event(d, e);
}

static void we_slider(
    void *d, SDL_Event *e)
{
    slider_event(d, e);
}

static void we_radio(
    void *d, SDL_Event *e)
{
    radio_event(d, e);
}

static void we_checkbox(
    void *d, SDL_Event *e)
{
    checkbox_event(d, e);
}

static void we_tabs(
    void *d, SDL_Event *e)
{
    tabs_event(d, e);
}

static void we_dropdown(
    void *d, SDL_Event *e)
{
    dropdown_event(d, e);
}

static void we_searchbar(
    void *d, SDL_Event *e)
{
    searchbar_event(d, e);
}

static void we_hamburger(
    void *d, SDL_Event *e)
{
    hamburger_event(d, e);
}

static void we_sidebar(
    void *d, SDL_Event *e)
{
    sidebar_event(d, e);
}

static void we_menubar(
    void *d, SDL_Event *e)
{
    menubar_event(d, e);
}

static void we_dialog(
    void *d, SDL_Event *e)
{
    dialog_box_event(d, e);
}

/* -------------------------------------------------- */
/*  Draw wrappers                                     */
/* -------------------------------------------------- */

static void wd_panel(
    void *d, PulsarUI *pui)
{
    panel_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_button(
    void *d, PulsarUI *pui)
{
    button_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_primary(
    void *d, PulsarUI *pui)
{
    primary_button_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_slider(
    void *d, PulsarUI *pui)
{
    slider_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_radio(
    void *d, PulsarUI *pui)
{
    radio_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_checkbox(
    void *d, PulsarUI *pui)
{
    checkbox_draw(
        d, &pui->ui,
        pui->renderer, pui->font);
}

static void wd_tabs(
    void *d, PulsarUI *pui)
{
    tabs_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font, pui->smooth_dt);
}

static void wd_dropdown(
    void *d, PulsarUI *pui)
{
    dropdown_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font, pui->smooth_dt);
}

static void wd_searchbar(
    void *d, PulsarUI *pui)
{
    searchbar_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font, pui->smooth_dt);
}

static void wd_hamburger(
    void *d, PulsarUI *pui)
{
    hamburger_draw(
        d, &pui->ui,
        pui->renderer);
}

static void wd_sidebar(
    void *d, PulsarUI *pui)
{
    sidebar_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font, pui->smooth_dt);
}

static void wd_progress(
    void *d, PulsarUI *pui)
{
    progress_bar_draw(
        d, &pui->ui,
        pui->renderer, pui->smooth_dt);
}

static void wd_menubar(
    void *d, PulsarUI *pui)
{
    menubar_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font, pui->smooth_dt);
}

/* -------------------------------------------------- */
/*  Layout wrappers                                   */
/* -------------------------------------------------- */

static void wl_panel(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    panel_layout(
        d, &pui->ui, x, y, w, h);
}

static void wl_button(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    button_layout(
        d, &pui->ui, x, y, w, h);
}

static void wl_primary(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    primary_button_layout(
        d, &pui->ui, x, y, w, h);
}

static void wl_slider(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)h;
    slider_layout(
        d, &pui->ui, x, y, w);
}

static void wl_radio(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)w; (void)h;
    radio_layout(
        d, &pui->ui, x, y);
}

static void wl_checkbox(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)w; (void)h;
    checkbox_layout(
        d, &pui->ui, x, y);
}

static void wl_tabs(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)h;
    tabs_layout(
        d, &pui->ui, x, y, w);
}

static void wl_dropdown(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)h;
    dropdown_layout(
        d, &pui->ui, x, y, w);
}

static void wl_searchbar(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    searchbar_layout(
        d, &pui->ui, x, y, w, h);
}

static void wl_hamburger(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)h;
    hamburger_layout(
        d, &pui->ui, x, y, w);
}

static void wl_sidebar(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    sidebar_layout(
        d, &pui->ui,
        x, y, w, h);
}

static void wl_menubar(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    (void)h;
    menubar_layout(
        d, &pui->ui, x, y, w);
}

static void wl_progress(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    progress_bar_layout(
        d, &pui->ui, x, y, w, h);
}

static void wl_image(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    ImageWidget *iw = d;
    iw->rect =
        ui_rect(&pui->ui, x, y, w, h);
}

static void wd_image(
    void *d, PulsarUI *pui)
{
    ImageWidget *iw = d;

    if (!iw->loaded)
        return;

    image_widget_draw(
        iw,
        pui->renderer,
        iw->rect.x,
        iw->rect.y,
        iw->rect.w,
        iw->rect.h
    );
}

/* -------------------------------------------------- */
/*  Public registration                               */
/* -------------------------------------------------- */

void pulsarui_add_panel(
    PulsarUI *pui, UIPanel *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        NULL, wd_panel, wl_panel,
        x, y, w2, h);
}

void pulsarui_add_button(
    PulsarUI *pui, UIButton *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_button, wd_button, wl_button,
        x, y, w2, h);
}

void pulsarui_add_primary_button(
    PulsarUI *pui,
    UIPrimaryButton *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_primary, wd_primary, wl_primary,
        x, y, w2, h);
}

void pulsarui_add_slider(
    PulsarUI *pui, UISlider *w,
    float x, float y, float w2)
{
    add_widget(pui, w,
        we_slider, wd_slider, wl_slider,
        x, y, w2, 0);
}

void pulsarui_add_radio(
    PulsarUI *pui, UIRadio *w,
    float x, float y)
{
    add_widget(pui, w,
        we_radio, wd_radio, wl_radio,
        x, y, 0, 0);
}

void pulsarui_add_checkbox(
    PulsarUI *pui, UICheckBox *w,
    float x, float y)
{
    add_widget(pui, w,
        we_checkbox, wd_checkbox,
        wl_checkbox,
        x, y, 0, 0);
}

void pulsarui_add_tabs(
    PulsarUI *pui, UITabs *w,
    float x, float y, float w2)
{
    add_widget(pui, w,
        we_tabs, wd_tabs, wl_tabs,
        x, y, w2, 0);
}

void pulsarui_add_dropdown(
    PulsarUI *pui, UIDropDown *w,
    float x, float y, float w2)
{
    add_widget(pui, w,
        we_dropdown, wd_dropdown,
        wl_dropdown,
        x, y, w2, 0);
}

void pulsarui_add_searchbar(
    PulsarUI *pui, UISearchBar *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_searchbar, wd_searchbar,
        wl_searchbar,
        x, y, w2, h);
}

void pulsarui_add_hamburger(
    PulsarUI *pui, UIHamburger *w,
    float x, float y, float size)
{
    add_widget(pui, w,
        we_hamburger, wd_hamburger,
        wl_hamburger,
        x, y, size, 0);
}

void pulsarui_add_sidebar(
    PulsarUI *pui, UISidebar *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_sidebar, wd_sidebar,
        wl_sidebar,
        x, y, w2, h);
}

void pulsarui_add_menubar(
    PulsarUI *pui, UIMenuBar *w,
    float x, float y, float w2)
{
    add_widget(pui, w,
        we_menubar, wd_menubar,
        wl_menubar,
        x, y, w2, 0);
}

void pulsarui_add_progress_bar(
    PulsarUI *pui,
    UIProgressBar *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        NULL, wd_progress, wl_progress,
        x, y, w2, h);
}

void pulsarui_add_dialog(
    PulsarUI *pui, DialogBox *w)
{
    add_widget(pui, w,
        we_dialog, NULL, NULL,
        0, 0, 0, 0);
}

void pulsarui_add_image(
    PulsarUI *pui,
    ImageWidget *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        NULL, wd_image, wl_image,
        x, y, w2, h);
}

static void wl_textinput(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    textinput_layout(
        d, &pui->ui, x, y, w, h);
}

static void we_textinput(
    void *d, SDL_Event *e)
{
    textinput_event(d, e);
}

static void wd_textinput(
    void *d, PulsarUI *pui)
{
    textinput_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font,
        pui->smooth_dt
    );
}

void pulsarui_add_textinput(
    PulsarUI *pui,
    UITextInput *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_textinput,
        wd_textinput,
        wl_textinput,
        x, y, w2, h
    );

    if (pui->focus_count < 64) {
        pui->focus_widgets[
            pui->focus_count++] = w;
    }
}

void pulsarui_add_orb(
    PulsarUI *pui,
    Orb *w,
    float x, float y,
    float w2, float h)
{
    (void)pui;
    w->dx = x;
    w->dy = y;
    w->dw = w2;
    w->dh = h;

    pui->top_orb = w;
}

static void wl_toggle(
    void *d, PulsarUI *pui,
    float x, float y,
    float w, float h)
{
    toggle_layout(d, &pui->ui, x, y, w, h);
}

static void we_toggle(
    void *d, SDL_Event *e)
{
    toggle_event(d, e);
}

static void wd_toggle(
    void *d, PulsarUI *pui)
{
    toggle_draw(
        d, &pui->ui,
        pui->renderer,
        pui->font);
}

void pulsarui_add_toggle(
    PulsarUI *pui,
    UIToggle *w,
    float x, float y,
    float w2, float h)
{
    add_widget(pui, w,
        we_toggle,
        wd_toggle,
        wl_toggle,
        x, y, w2, h
    );

    pui->theme_toggle = w;
}

void pulsarui_focus_next(
    PulsarUI *pui)
{
    if (pui->focus_count == 0)
        return;

    pui->focus_idx++;

    if (pui->focus_idx >=
        pui->focus_count)
        pui->focus_idx = 0;
}

void pulsarui_focus_prev(
    PulsarUI *pui)
{
    if (pui->focus_count == 0)
        return;

    pui->focus_idx--;

    if (pui->focus_idx < 0)
        pui->focus_idx =
            pui->focus_count - 1;
}

/* -------------------------------------------------- */
/*  Link helpers                                      */
/* -------------------------------------------------- */

void pulsarui_link_sidebar(
    PulsarUI *pui,
    UISidebar *sidebar,
    UIHamburger *hamburger)
{
    pui->sidebar = sidebar;
    pui->hamburger = hamburger;
}

void pulsarui_link_progress(
    PulsarUI *pui,
    UISlider *slider,
    UIProgressBar *bar)
{
    pui->progress_slider = slider;
    pui->progress_bar = bar;
}

/* -------------------------------------------------- */
/*  Lifecycle                                         */
/* -------------------------------------------------- */

int pulsarui_init(
    PulsarUI *pui,
    const char *title,
    int width,
    int height)
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

    pui->window =
        SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_RESIZABLE
        );

    pui->renderer =
        SDL_CreateRenderer(
            pui->window,
            -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC
        );

    if (!pui->window || !pui->renderer)
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

    pui->font =
        TTF_OpenFont(
            fontPath,
            19
        );

    pui->titleFont =
        TTF_OpenFont(
            fontPath,
            36
        );

    free(fontPath);

    if (!pui->font || !pui->titleFont) {

        printf(
            "Could not load font\n"
        );

        return 1;
    }

    pui->ui.dark = false;
    pui->rt = NULL;
    pui->rt_w = 0;
    pui->rt_h = 0;
    pui->bg = NULL;
    pui->bg_w = 0;
    pui->bg_h = 0;
    pui->bg_dark = false;
    pui->fade_alpha = 0.0f;
    pui->fade_target = 0.0f;
    pui->fade_switched = false;
    pui->theme_toggle = NULL;
    pui->start =
        SDL_GetPerformanceCounter();
    pui->time = 0.0f;
    pui->dt = 0.0f;
    pui->smooth_dt = 0.016f;
    pui->dt_init = false;
    pui->running = true;
    pui->widget_count = 0;
    pui->widget_cap = 0;
    pui->widgets = NULL;

    pui->sidebar = NULL;
    pui->hamburger = NULL;
    pui->progress_slider = NULL;
    pui->progress_bar = NULL;

    pui->focus_count = 0;
    pui->focus_idx = 0;

    pui->top_orb = NULL;

    tooltip_manager_init(
        &pui->tooltips
    );

    notif_manager_init(
        &pui->notifications
    );

    return 0;
}

bool pulsarui_begin(
    PulsarUI *pui)
{
    if (!pui->running)
        return false;

    int width;
    int height;

    SDL_GetWindowSize(
        pui->window,
        &width,
        &height
    );

    ui_begin(
        &pui->ui,
        width,
        height
    );

    Uint64 now =
        SDL_GetPerformanceCounter();

    pui->time =
        (float)(
            (double)(
                now - pui->start
            ) /
            SDL_GetPerformanceFrequency()
        );

    static float prev_time = 0.0f;
    pui->dt = pui->time - prev_time;
    prev_time = pui->time;

    if (!pui->dt_init) {
        pui->smooth_dt = pui->dt;
        pui->dt_init = true;
    } else {
        pui->smooth_dt =
            pui->dt * 0.25f +
            pui->smooth_dt * 0.75f;
    }

    if (pui->rt &&
        (width != pui->rt_w ||
         height != pui->rt_h)) {

        SDL_DestroyTexture(pui->rt);

        pui->rt_w = width;
        pui->rt_h = height;

        pui->rt =
            SDL_CreateTexture(
                pui->renderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_TARGET,
                width * 2,
                height * 2);

        SDL_SetTextureBlendMode(
            pui->rt,
            SDL_BLENDMODE_BLEND
        );
    }

    if (!pui->rt) {

        pui->rt_w = width;
        pui->rt_h = height;

        pui->rt =
            SDL_CreateTexture(
                pui->renderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_TARGET,
                width * 2,
                height * 2);

        SDL_SetTextureBlendMode(
            pui->rt,
            SDL_BLENDMODE_BLEND
        );
    }

    SDL_SetRenderTarget(
        pui->renderer,
        pui->rt
    );

    SDL_RenderSetScale(
        pui->renderer,
        2.0f,
        2.0f
    );

    bool bg_dirty =
        pui->bg_w != width * 2 ||
        pui->bg_h != height * 2 ||
        pui->bg_dark != pui->ui.dark;

    if (bg_dirty) {

        if (pui->bg)
            SDL_DestroyTexture(pui->bg);

        pui->bg_w = width * 2;
        pui->bg_h = height * 2;
        pui->bg_dark = pui->ui.dark;

        pui->bg =
            SDL_CreateTexture(
                pui->renderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_TARGET,
                width * 2,
                height * 2);

        SDL_SetTextureBlendMode(
            pui->bg,
            SDL_BLENDMODE_BLEND
        );

        SDL_SetRenderTarget(
            pui->renderer,
            pui->bg
        );

        draw_background_base(
            pui->renderer,
            width,
            height,
            pui->ui.dark
        );

        SDL_SetRenderTarget(
            pui->renderer,
            pui->rt
        );
    }

    SDL_RenderCopy(
        pui->renderer,
        pui->bg,
        NULL,
        NULL
    );

    draw_background_orbs(
        pui->renderer,
        width,
        height,
        pui->time,
        pui->ui.dark
    );

    for (int i = 0;
         i < pui->widget_count;
         i++) {

        PulsarWidget *wd =
            &pui->widgets[i];

        if (wd->on_layout)
            wd->on_layout(
                wd->data, pui,
                wd->x, wd->y,
                wd->w, wd->h
            );
    }

    return true;
}

void pulsarui_end(
    PulsarUI *pui)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT)
            pui->running = false;

        if (
            event.type == SDL_KEYDOWN &&
            event.key.keysym.sym ==
            SDLK_ESCAPE
        )
            pui->running = false;

        if (
            event.type == SDL_KEYDOWN &&
            event.key.keysym.sym ==
            SDLK_d
        )
        {
            if (pui->theme_toggle) {
                pui->theme_toggle->on =
                    !pui->theme_toggle->on;
            }

            if (pui->fade_target == 0.0f) {
                pui->fade_target = 1.0f;
                pui->fade_switched = false;
            }
        }

        for (int i = 0;
             i < pui->widget_count;
             i++) {

            PulsarWidget *wd =
                &pui->widgets[i];

            if (wd->on_event)
                wd->on_event(
                    wd->data, &event);
        }

        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym ==
            SDLK_TAB)
        {
            bool shift =
                event.key.keysym.mod &
                KMOD_SHIFT;

            if (shift)
                pulsarui_focus_prev(pui);
            else
                pulsarui_focus_next(pui);
        }
    }

    if (pui->hamburger && pui->sidebar) {

        if (pui->hamburger->clicked)
            pui->sidebar->open =
                !pui->sidebar->open;

        pui->hamburger->open =
            pui->sidebar->open;
    }

    if (pui->theme_toggle &&
        pui->theme_toggle->clicked)
    {
        if (pui->fade_target == 0.0f) {
            pui->fade_target = 1.0f;
            pui->fade_switched = false;
        }
    }

    if (pui->fade_target != 0.0f ||
        pui->fade_alpha > 0.001f)
    {
        float diff =
            pui->fade_target - pui->fade_alpha;

        if (fabsf(diff) < 0.01f) {

            pui->fade_alpha = pui->fade_target;

            if (!pui->fade_switched &&
                pui->fade_target > 0.5f)
            {
                pui->ui.dark =
                    !pui->ui.dark;
                ui_text_cache_clear();
                pui->fade_switched = true;
                pui->fade_target = 0.0f;
            }
        } else {
            pui->fade_alpha +=
                diff * 8.0f * pui->smooth_dt;
        }
    }

    if (pui->progress_slider &&
        pui->progress_bar) {

        progress_bar_set_value(
            pui->progress_bar,
            pui->progress_slider->value
        );
    }

    tooltip_update(
        &pui->tooltips,
        NULL, pui->smooth_dt
    );

    notif_update(
        &pui->notifications,
        pui->smooth_dt
    );

    for (int i = 0;
         i < pui->widget_count;
         i++) {

        PulsarWidget *wd =
            &pui->widgets[i];

        if (wd->on_draw)
            wd->on_draw(
                wd->data, pui);
    }

    if (pui->top_orb &&
        pui->top_orb->visible)
    {
        orb_update(
            pui->top_orb,
            pui->smooth_dt
        );

        pui->top_orb->rect =
            ui_rect(
                &pui->ui,
                pui->top_orb->dx,
                pui->top_orb->dy,
                pui->top_orb->dw,
                pui->top_orb->dh
            );

        orb_draw(
            pui->top_orb,
            pui->renderer,
            pui->top_orb->rect
        );
    }

    tooltip_draw(
        &pui->tooltips,
        &pui->ui,
        pui->renderer,
        pui->font
    );

    int width;
    int height;

    SDL_GetWindowSize(
        pui->window,
        &width,
        &height
    );

    SDL_SetRenderTarget(
        pui->renderer,
        NULL
    );

    SDL_RenderSetScale(
        pui->renderer,
        1.0f,
        1.0f
    );

    SDL_Rect dst = {
        0, 0, width, height
    };

    SDL_RenderCopy(
        pui->renderer,
        pui->rt,
        NULL,
        &dst
    );

    if (pui->fade_alpha > 0.01f) {

        SDL_SetRenderDrawBlendMode(
            pui->renderer,
            SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(
            pui->renderer,
            0, 0, 0,
            (Uint8)(pui->fade_alpha * 255.0f));

        SDL_RenderFillRect(
            pui->renderer,
            &dst);
    }
}

void pulsarui_present(
    PulsarUI *pui)
{
    notif_draw(
        &pui->notifications,
        &pui->ui,
        pui->renderer,
        pui->font
    );

    SDL_RenderPresent(
        pui->renderer
    );
}

void pulsarui_shutdown(
    PulsarUI *pui)
{
    TTF_CloseFont(pui->font);
    TTF_CloseFont(pui->titleFont);

    ui_text_cache_shutdown();

    if (pui->rt)
        SDL_DestroyTexture(pui->rt);

    if (pui->bg)
        SDL_DestroyTexture(pui->bg);

    free(pui->widgets);

    SDL_DestroyRenderer(
        pui->renderer
    );

    SDL_DestroyWindow(
        pui->window
    );

    TTF_Quit();
    SDL_Quit();
}
