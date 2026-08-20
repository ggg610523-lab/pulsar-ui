# Core API Reference

## ui.h — Primitives & Context

### Types

```c
typedef struct {
    Uint8 r, g, b, a;
} UIColor;

typedef struct {
    float scale;       // ui->scale = base_w / window_w
    int window_w;
    int window_h;
    float base_w;      // design width  (960)
    float base_h;      // design height (720)
    bool dark;
} UIContext;
```

### Functions

#### ui_begin
```c
void ui_begin(UIContext *ui, int width, int height);
```
Initialize context. Called once at startup. Sets `scale`, `window_w`, `window_h`, `base_w`, `base_h`.

---

#### ui_rect
```c
SDL_Rect ui_rect(UIContext *ui, float x, float y, float w, float h);
```
Convert design-space coordinates to a scaled `SDL_Rect`. All `pulsarui_add_*()` calls accept design coords; this function is used internally but can also be called directly when drawing native-res overlays.

---

#### ui_point_in_rect
```c
bool ui_point_in_rect(int x, int y, SDL_Rect rect);
```
Hit-test a point against a rect.

---

#### ui_fill_rounded_rect
```c
void ui_fill_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color
);
```
Scanline-filled rounded rectangle with anti-aliased edge pixels.

---

#### ui_fill_rounded_rect_gradient
```c
void ui_fill_rounded_rect_gradient(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor top,
    UIColor bottom
);
```
Vertical gradient rounded rectangle.

---

#### ui_fill_gradient_rect
```c
void ui_fill_gradient_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor top,
    UIColor bottom
);
```
Vertical gradient rectangle (no rounding).

---

#### ui_fill_radial_gradient
```c
void ui_fill_radial_gradient(
    SDL_Renderer *renderer,
    int cx, int cy, int radius,
    UIColor center,
    UIColor edge
);
```
Radial gradient circle fill.

---

#### ui_fill_circle
```c
void ui_fill_circle(
    SDL_Renderer *renderer,
    int cx, int cy, int radius,
    UIColor color
);
```
Scanline-filled circle.

---

#### ui_fill_capsule
```c
void ui_fill_capsule(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    UIColor color
);
```
Capsule shape (fully rounded ends).

---

#### ui_outline_rounded_rect
```c
void ui_outline_rounded_rect(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    UIColor color
);
```
1px rounded rectangle outline.

---

#### ui_glass
```c
void ui_glass(
    SDL_Renderer *renderer,
    SDL_Rect rect,
    int radius,
    bool active,
    bool dark
);
```
Glassmorphism panel: translucent fill, rounded rect, subtle inner highlight, and themed outline. `active` controls the outline highlight intensity.

---

#### ui_color_lerp
```c
UIColor ui_color_lerp(UIColor a, UIColor b, float t);
```
Linearly interpolate between two colors. `t` clamped to [0, 1].

---

#### ui_theme
```c
UIColor ui_theme(bool dark, UIColor light, UIColor dark_color);
```
Return `light` if `dark == false`, otherwise `dark_color`. The primary theming helper.

---

#### ui_ease_out_cubic
```c
float ui_ease_out_cubic(float t);
```
Easing: `1 - (1-t)^3`.

---

#### ui_ease_in_out_cubic
```c
float ui_ease_in_out_cubic(float t);
```
Easing: smooth ease in-out.

---

#### ui_text
```c
void ui_text(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x, int y,
    UIColor color
);
```
Render cached text at (x, y). Text textures are cached across frames (256 entries max). Call `ui_text_cache_clear()` to invalidate on theme change.

---

#### ui_text_shadow
```c
void ui_text_shadow(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    int x, int y,
    UIColor color,
    UIColor shadow,
    int offset
);
```
Render text with a 1px shadow at (x+offset, y+offset).

---

#### ui_text_center
```c
void ui_text_center(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const char *text,
    SDL_Rect rect,
    UIColor color
);
```
Center text horizontally and vertically within a rect.

---

#### ui_text_cache_clear
```c
void ui_text_cache_clear(void);
```
Destroy all cached text textures. Call on theme toggle to force re-render with new colors.

---

#### ui_text_cache_shutdown
```c
void ui_text_cache_shutdown(void);
```
Free all cached text textures. Call at program exit.

---

## background.h — Animated Background

```c
void draw_background_base(
    SDL_Renderer *renderer,
    int width, int height,
    bool dark
);
```
Static gradient base. Cached in PulsarUI; only redrawn on resize or theme change.

```c
void draw_background_orbs(
    SDL_Renderer *renderer,
    int width, int height,
    float time,
    bool dark
);
```
Animated floating orb blurs. Drawn every frame.

```c
void draw_background(
    SDL_Renderer *renderer,
    int width, int height,
    float time,
    bool dark
);
```
Convenience: base + orbs in one call.

---

## pulsarUI.h — Application Framework

### Types

```c
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *titleFont;
    UIContext ui;
    SDL_Texture *rt;          // 2x supersampled render target
    int rt_w, rt_h;
    SDL_Texture *bg;          // cached background base
    int bg_w, bg_h;
    bool bg_dark;
    Uint64 start;
    float time;
    float dt;
    float smooth_dt;          // EMA-smoothed dt
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
    float fade_alpha;         // dark mode transition
    float fade_target;
    bool fade_switched;
    UIToggle *theme_toggle;
    Orb *top_orb;
} PulsarUI;

typedef struct PulsarWidget {
    void *data;
    PulsarEventFn on_event;   // void(*)(void*, SDL_Event*)
    PulsarDrawFn on_draw;     // void(*)(void*, PulsarUI*)
    PulsarLayoutFn on_layout; // void(*)(void*, PulsarUI*, float, float, float, float)
    float x, y, w, h;
} PulsarWidget;
```

### Lifecycle

```c
int pulsarui_init(PulsarUI *pui, const char *title, int width, int height);
```
Create window, renderer, fonts, render target. Returns 0 on success.

```c
bool pulsarui_begin(PulsarUI *pui);
```
Start frame. Returns false when window is closed. Clears render target, draws background, dispatches SDL events to all widgets.

```c
void pulsarui_end(PulsarUI *pui);
```
End frame. Draws all registered widgets, orb, tooltips, notifications. Applies dark mode fade overlay. Downscales 2x render target to window.

```c
void pulsarui_present(PulsarUI *pui);
```
Present the rendered frame. Call after drawing any native-res overlays (text, glass panels).

```c
void pulsarui_shutdown(PulsarUI *pui);
```
Free all resources.

### Widget Registration

All `pulsarui_add_*()` functions take design-space coordinates (x, y, w, h) and auto-wire event, layout, and draw callbacks.

```c
void pulsarui_add_panel(PulsarUI *pui, UIPanel *w, float x, float y, float w2, float h);
void pulsarui_add_button(PulsarUI *pui, UIButton *w, float x, float y, float w2, float h);
void pulsarui_add_primary_button(PulsarUI *pui, UIPrimaryButton *w, float x, float y, float w2, float h);
void pulsarui_add_slider(PulsarUI *pui, UISlider *w, float x, float y, float w2);
void pulsarui_add_radio(PulsarUI *pui, UIRadio *w, float x, float y);
void pulsarui_add_checkbox(PulsarUI *pui, UICheckBox *w, float x, float y);
void pulsarui_add_tabs(PulsarUI *pui, UITabs *w, float x, float y, float w2);
void pulsarui_add_dropdown(PulsarUI *pui, UIDropDown *w, float x, float y, float w2);
void pulsarui_add_searchbar(PulsarUI *pui, UISearchBar *w, float x, float y, float w2, float h);
void pulsarui_add_hamburger(PulsarUI *pui, UIHamburger *w, float x, float y, float size);
void pulsarui_add_sidebar(PulsarUI *pui, UISidebar *w, float x, float y, float w2, float h);
void pulsarui_add_menubar(PulsarUI *pui, UIMenuBar *w, float x, float y, float w2);
void pulsarui_add_progress_bar(PulsarUI *pui, UIProgressBar *w, float x, float y, float w2, float h);
void pulsarui_add_dialog(PulsarUI *pui, DialogBox *w);
void pulsarui_add_image(PulsarUI *pui, ImageWidget *w, float x, float y, float w2, float h);
void pulsarui_add_textinput(PulsarUI *pui, UITextInput *w, float x, float y, float w2, float h);
void pulsarui_add_orb(PulsarUI *pui, Orb *w, float x, float y, float w2, float h);
void pulsarui_add_toggle(PulsarUI *pui, UIToggle *w, float x, float y, float w2, float h);
```

### Linking Helpers

```c
void pulsarui_link_sidebar(PulsarUI *pui, UISidebar *sidebar, UIHamburger *hamburger);
```
Wire hamburger button to open/close sidebar.

```c
void pulsarui_link_progress(PulsarUI *pui, UISlider *slider, UIProgressBar *bar);
```
Bind slider value to progress bar display.

### Focus Navigation

```c
void pulsarui_focus_next(PulsarUI *pui);
void pulsarui_focus_prev(PulsarUI *pui);
```
Cycle focus among focusable widgets (TextInput, SearchBar). Bound to Tab/Shift+Tab internally.
