#ifndef PULSAR_UI_H
#define PULSAR_UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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
#include "background.h"
#include "imageWidget.h"
#include "textinput.h"
#include "orb.h"
#include "toggle.h"

#define PULSAR_MAX_WIDGETS 64

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *titleFont;
    UIContext ui;

    SDL_Texture *rt;
    int rt_w;
    int rt_h;

    SDL_Texture *bg;
    int bg_w;
    int bg_h;
    bool bg_dark;

    Uint64 start;
    float time;
    float dt;
    float smooth_dt;
    bool dt_init;
    bool running;

    struct PulsarWidget *widgets;
    int widget_count;
    int widget_cap;

    UITooltipManager tooltips;
    NotificationManager notifications;

    UISidebar *sidebar;
    UIHamburger *hamburger;
    UISlider *progress_slider;
    UIProgressBar *progress_bar;

    int focus_count;
    void *focus_widgets[64];
    int focus_idx;

    float fade_alpha;
    float fade_target;
    bool fade_switched;

    UIToggle *theme_toggle;

    Orb *top_orb;
} PulsarUI;

typedef void (*PulsarEventFn)(
    void *data, SDL_Event *event
);

typedef void (*PulsarDrawFn)(
    void *data, PulsarUI *pui
);

typedef void (*PulsarLayoutFn)(
    void *data, PulsarUI *pui,
    float x, float y,
    float w, float h
);

typedef struct PulsarWidget {
    void *data;
    PulsarEventFn on_event;
    PulsarDrawFn on_draw;
    PulsarLayoutFn on_layout;
    float x, y, w, h;
} PulsarWidget;

int pulsarui_init(
    PulsarUI *pui,
    const char *title,
    int width,
    int height
);

void pulsarui_add_panel(
    PulsarUI *pui, UIPanel *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_button(
    PulsarUI *pui, UIButton *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_primary_button(
    PulsarUI *pui,
    UIPrimaryButton *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_slider(
    PulsarUI *pui, UISlider *w,
    float x, float y, float w2
);

void pulsarui_add_radio(
    PulsarUI *pui, UIRadio *w,
    float x, float y
);

void pulsarui_add_checkbox(
    PulsarUI *pui, UICheckBox *w,
    float x, float y
);

void pulsarui_add_tabs(
    PulsarUI *pui, UITabs *w,
    float x, float y, float w2
);

void pulsarui_add_dropdown(
    PulsarUI *pui, UIDropDown *w,
    float x, float y, float w2
);

void pulsarui_add_searchbar(
    PulsarUI *pui, UISearchBar *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_hamburger(
    PulsarUI *pui, UIHamburger *w,
    float x, float y, float size
);

void pulsarui_add_sidebar(
    PulsarUI *pui, UISidebar *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_menubar(
    PulsarUI *pui, UIMenuBar *w,
    float x, float y, float w2
);

void pulsarui_add_progress_bar(
    PulsarUI *pui,
    UIProgressBar *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_dialog(
    PulsarUI *pui, DialogBox *w
);

void pulsarui_add_image(
    PulsarUI *pui,
    ImageWidget *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_textinput(
    PulsarUI *pui,
    UITextInput *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_orb(
    PulsarUI *pui,
    Orb *w,
    float x, float y,
    float w2, float h
);

void pulsarui_add_toggle(
    PulsarUI *pui,
    UIToggle *w,
    float x, float y,
    float w2, float h
);

void pulsarui_focus_next(
    PulsarUI *pui
);

void pulsarui_focus_prev(
    PulsarUI *pui
);

void pulsarui_link_sidebar(
    PulsarUI *pui,
    UISidebar *sidebar,
    UIHamburger *hamburger
);

void pulsarui_link_progress(
    PulsarUI *pui,
    UISlider *slider,
    UIProgressBar *bar
);

bool pulsarui_begin(
    PulsarUI *pui
);

void pulsarui_end(
    PulsarUI *pui
);

void pulsarui_present(
    PulsarUI *pui
);

void pulsarui_shutdown(
    PulsarUI *pui
);

#endif /* PULSAR_UI_H */
