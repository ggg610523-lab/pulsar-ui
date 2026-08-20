# Widget API Reference

Each widget follows the same pattern:
1. `*_init()` — initialize with default values
2. `*_layout()` — set position (called internally by `pulsarui_add_*`)
3. `*_event()` — handle SDL events (called internally by framework)
4. `*_draw()` — render (called internally by framework)

All coordinates below are in **design space** (960×720). The framework scales them automatically.

---

## Panel

`#include "panel.h"`

Glassmorphism container with a title label.

```c
typedef struct {
    SDL_Rect rect;
    const char *title;
} UIPanel;
```

| Function | Description |
|----------|-------------|
| `panel_init(UIPanel *panel, const char *title)` | Initialize with title |
| `panel_layout(UIPanel *panel, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `panel_draw(UIPanel *panel, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw glass panel + title |

---

## Button

`#include "button.h"`

Glass-style button with hover animation.

```c
typedef struct {
    SDL_Rect rect;
    const char *text;
    bool hovered;
    bool pressed;
    bool clicked;       // true for one frame after click
    float hover_anim;
} UIButton;
```

| Function | Description |
|----------|-------------|
| `button_init(UIButton *button, const char *text)` | Initialize with label |
| `button_layout(UIButton *button, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `button_event(UIButton *button, SDL_Event *event)` | Handle mouse events |
| `button_draw(UIButton *button, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw button |

**Usage:** Check `button.clicked` each frame for click detection.

---

## Primary Button

`#include "primaryButton.h"`

Filled accent button (blue in light mode, blue in dark mode).

```c
typedef struct {
    SDL_Rect rect;
    const char *text;
    bool hovered;
    bool pressed;
    bool clicked;
    float hover_anim;
} UIPrimaryButton;
```

| Function | Description |
|----------|-------------|
| `primary_button_init(UIPrimaryButton *button, const char *text)` | Initialize with label |
| `primary_button_layout(UIPrimaryButton *button, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `primary_button_event(UIPrimaryButton *button, SDL_Event *event)` | Handle mouse events |
| `primary_button_draw(UIPrimaryButton *button, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw button |

---

## Slider

`#include "slider.h"`

Horizontal slider with draggable thumb.

```c
typedef struct {
    SDL_Rect rect;
    float value;        // 0.0 – 1.0
    bool dragging;
} UISlider;
```

| Function | Description |
|----------|-------------|
| `slider_init(UISlider *slider, float value)` | Initialize with initial value (0–1) |
| `slider_layout(UISlider *slider, UIContext *ui, float x, float y, float w)` | Set position (height is auto) |
| `slider_event(UISlider *slider, SDL_Event *event)` | Handle drag events |
| `slider_draw(UISlider *slider, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw slider |

**Usage:** Read `slider.value` each frame. Link to a progress bar with `pulsarui_link_progress()`.

---

## Radio

`#include "radio.h"`

Radio button with toggle selection.

```c
typedef struct {
    SDL_Rect rect;
    const char *text;
    bool selected;
} UIRadio;
```

| Function | Description |
|----------|-------------|
| `radio_init(UIRadio *radio, const char *text, bool selected)` | Initialize with label and initial state |
| `radio_layout(UIRadio *radio, UIContext *ui, float x, float y)` | Set position (size is auto) |
| `radio_event(UIRadio *radio, SDL_Event *event)` | Handle click; returns true if state changed |
| `radio_draw(UIRadio *radio, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw radio button |

**Usage:** Manually manage mutual exclusion — deselect other radios when one is selected.

---

## Checkbox

`#include "checkbox.h"`

Toggle checkbox with checkmark animation.

```c
typedef struct {
    SDL_Rect rect;
    const char *text;
    bool checked;
} UICheckBox;
```

| Function | Description |
|----------|-------------|
| `checkbox_init(UICheckBox *checkbox, const char *text, bool checked)` | Initialize with label and state |
| `checkbox_layout(UICheckBox *checkbox, UIContext *ui, float x, float y)` | Set position (size is auto) |
| `checkbox_event(UICheckBox *checkbox, SDL_Event *event)` | Handle click |
| `checkbox_draw(UICheckBox *checkbox, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw checkbox |

---

## Search Bar

`#include "searchBar.h"`

Text input with search icon and cursor blink.

```c
#define SEARCHBAR_MAX 256

typedef struct {
    SDL_Rect rect;
    char text[SEARCHBAR_MAX];
    int len;
    bool focused;
    float cursor_blink;
} UISearchBar;
```

| Function | Description |
|----------|-------------|
| `searchbar_init(UISearchBar *bar)` | Initialize empty |
| `searchbar_layout(UISearchBar *bar, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `searchbar_event(UISearchBar *bar, SDL_Event *event)` | Handle keyboard/mouse events |
| `searchbar_draw(UISearchBar *bar, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw search bar (needs dt for cursor blink) |

---

## Text Input

`#include "textinput.h"`

Full-featured text input with cursor, selection, clipboard support.

```c
#define TEXTINPUT_MAX 512

typedef struct {
    char text[TEXTINPUT_MAX];
    int cursor;
    int sel_start;
    int sel_end;
    bool focused;
    bool hovered;
    float cursor_blink;
    float scroll_x;
    SDL_Rect rect;
    SDL_Rect text_area;
    int font_size;
} UITextInput;
```

| Function | Description |
|----------|-------------|
| `textinput_init(UITextInput *ti, int font_size)` | Initialize with font size |
| `textinput_layout(UITextInput *ti, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `textinput_event(UITextInput *ti, SDL_Event *e)` | Handle keyboard/mouse/clipboard |
| `textinput_draw(UITextInput *ti, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw input |
| `textinput_get(UITextInput *ti)` | Get current text (const char*) |
| `textinput_set(UITextInput *ti, const char *text)` | Set text programmatically |
| `textinput_clear(UITextInput *ti)` | Clear all text |
| `textinput_submit(UITextInput *ti)` | Returns true when Enter is pressed |

**Features:** Ctrl+A select all, Ctrl+C copy, Ctrl+V paste, Ctrl+X cut, arrow keys, Shift+arrows selection, Home/End.

---

## Toggle

`#include "toggle.h"`

Animated toggle switch with dark/light label.

```c
typedef struct {
    SDL_Rect rect;
    bool on;
    bool clicked;
    float thumb_x;
    float anim;
} UIToggle;
```

| Function | Description |
|----------|-------------|
| `toggle_init(UIToggle *toggle, bool on)` | Initialize with initial state |
| `toggle_layout(UIToggle *toggle, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `toggle_event(UIToggle *toggle, SDL_Event *event)` | Handle click |
| `toggle_draw(UIToggle *toggle, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw toggle |

**Special:** When registered with `pulsarui_add_toggle()`, the framework auto-wires it as the theme toggle — clicking it triggers the dark mode fade transition and clears the text cache.

---

## Tabs

`#include "tabs.h"`

Tab bar with animated indicator.

```c
#define TABS_MAX 8

typedef struct {
    SDL_Rect rect;
    SDL_Rect tabRects[TABS_MAX];
    const char *labels[TABS_MAX];
    int count;
    int selected;
    float indicatorX;
    float indicatorW;
} UITabs;
```

| Function | Description |
|----------|-------------|
| `tabs_init(UITabs *tabs)` | Initialize empty |
| `tabs_add(UITabs *tabs, const char *label)` | Add a tab (max 8) |
| `tabs_layout(UITabs *tabs, UIContext *ui, float x, float y, float w)` | Set position (height is auto) |
| `tabs_event(UITabs *tabs, SDL_Event *event)` | Handle tab clicks |
| `tabs_draw(UITabs *tabs, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw tabs |

**Usage:** Check `tabs.selected` each frame for the active tab index.

---

## Menu Bar

`#include "menuBar.h"`

Horizontal menu bar with dropdown menus.

```c
#define MENUBAR_MAX_MENUS 8
#define MENUBAR_MAX_ITEMS 16

typedef struct {
    const char *label;
} MenuItem;

typedef struct {
    const char *label;
    MenuItem items[MENUBAR_MAX_ITEMS];
    int itemCount;
    SDL_Rect rect;
    SDL_Rect menuRect;
} Menu;

typedef struct {
    SDL_Rect rect;
    Menu menus[MENUBAR_MAX_MENUS];
    int menuCount;
    int openMenu;
    float anim;
} UIMenuBar;
```

| Function | Description |
|----------|-------------|
| `menubar_init(UIMenuBar *bar)` | Initialize empty |
| `menubar_add_menu(UIMenuBar *bar, const char *label)` | Add a top-level menu |
| `menubar_add_item(UIMenuBar *bar, int menuIndex, const char *item)` | Add item to a menu |
| `menubar_layout(UIMenuBar *bar, UIContext *ui, float x, float y, float w)` | Set position (height is auto) |
| `menubar_event(UIMenuBar *bar, SDL_Event *event)` | Handle clicks |
| `menubar_selected(UIMenuBar *bar, int *menuIdx, int *itemIdx)` | Returns true if an item was just selected, outputs indices |
| `menubar_draw(UIMenuBar *bar, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw menu bar |

---

## Sidebar

`#include "sidebar.h"`

Slide-in sidebar with fixed items.

```c
#define SIDEBAR_ITEMS 4

typedef struct {
    SDL_Rect rect;
    float anim;
    bool open;
    const char *items[SIDEBAR_ITEMS];
} UISidebar;
```

| Function | Description |
|----------|-------------|
| `sidebar_init(UISidebar *sb)` | Initialize closed |
| `sidebar_layout(UISidebar *sb, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `sidebar_event(UISidebar *sb, SDL_Event *event)` | Handle events |
| `sidebar_draw(UISidebar *sb, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw sidebar |

**Usage:** Link with `pulsarui_link_sidebar()` to auto-wire a hamburger button.

---

## Hamburger

`#include "hamburger.h"`

Three-line menu button that toggles open state.

```c
typedef struct {
    SDL_Rect rect;
    bool open;
    bool clicked;
    float anim;
} UIHamburger;
```

| Function | Description |
|----------|-------------|
| `hamburger_init(UIHamburger *hb)` | Initialize closed |
| `hamburger_layout(UIHamburger *hb, UIContext *ui, float x, float y, float size)` | Set position (square) |
| `hamburger_event(UIHamburger *hb, SDL_Event *event)` | Handle click |
| `hamburger_draw(UIHamburger *hb, UIContext *ui, SDL_Renderer *renderer)` | Draw hamburger icon |

---

## Dropdown Menu

`#include "dropdownmenu.h"`

Dropdown selector with animated list.

```c
#define DROPDOWN_MAX_ITEMS 16

typedef struct {
    SDL_Rect rect;
    SDL_Rect listRect;
    const char *items[DROPDOWN_MAX_ITEMS];
    int itemCount;
    int selected;
    bool open;
    bool hovered;
    float anim;
} UIDropDown;
```

| Function | Description |
|----------|-------------|
| `dropdown_init(UIDropDown *dd)` | Initialize empty |
| `dropdown_add_item(UIDropDown *dd, const char *item)` | Add an option |
| `dropdown_layout(UIDropDown *dd, UIContext *ui, float x, float y, float w)` | Set position (height is auto) |
| `dropdown_event(UIDropDown *dd, SDL_Event *event)` | Handle clicks |
| `dropdown_draw(UIDropDown *dd, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font, float dt)` | Draw dropdown |

**Usage:** Check `dropdown.selected` for the selected index, `dropdown.items[dropdown.selected]` for the label.

---

## Progress Bar

`#include "progressBar.h"`

Animated progress bar with smooth value interpolation.

```c
typedef struct {
    SDL_Rect rect;
    float value;          // target value (0–1)
    float displayValue;   // animated display value
} UIProgressBar;
```

| Function | Description |
|----------|-------------|
| `progress_bar_init(UIProgressBar *bar, float value)` | Initialize with value (0–1) |
| `progress_bar_layout(UIProgressBar *bar, UIContext *ui, float x, float y, float w, float h)` | Set position/size |
| `progress_bar_draw(UIProgressBar *bar, UIContext *ui, SDL_Renderer *renderer, float dt)` | Draw progress bar |
| `progress_bar_set_value(UIProgressBar *bar, float value)` | Set target value |

**Usage:** Link with `pulsarui_link_progress()` to auto-track a slider.

---

## Dialog Box

`#include "dialogBox.h"`

Modal dialog with its own SDL window. Separate from the main PulsarUI render target.

```c
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    bool open;
    bool closing;
    bool dark;
    float anim;
    bool dragging;
    int drag_off_x;
    int drag_off_y;
    UIButton cancelBtn;
    UIPrimaryButton okBtn;
} DialogBox;
```

| Function | Description |
|----------|-------------|
| `dialog_box_init(DialogBox *dlg, TTF_Font *font)` | Initialize (creates hidden window) |
| `dialog_box_open(DialogBox *dlg)` | Show dialog |
| `dialog_box_event(DialogBox *dlg, SDL_Event *event)` | Handle events (drag, buttons) |
| `dialog_box_draw(DialogBox *dlg, bool dark, float dt)` | Draw dialog (call after `pulsarui_present()`) |
| `dialog_box_close(DialogBox *dlg)` | Close and destroy window |

**Note:** Dialog runs in its own window with its own renderer. Draw it after `pulsarui_present()` in the main loop.

---

## Notifications

`#include "notifications.h"`

Toast notification stack with fade-in/out.

```c
#define NOTIF_MAX 8
#define NOTIF_TEXT_MAX 128

typedef struct {
    char text[NOTIF_TEXT_MAX];
    float alpha;
    float timer;
    bool active;
} Notification;

typedef struct {
    Notification items[NOTIF_MAX];
    int count;
} NotificationManager;
```

| Function | Description |
|----------|-------------|
| `notif_manager_init(NotificationManager *mgr)` | Initialize empty |
| `notif_push(NotificationManager *mgr, const char *text)` | Push a notification (auto-fades after ~3s) |
| `notif_update(NotificationManager *mgr, float dt)` | Update timers (called by framework) |
| `notif_draw(NotificationManager *mgr, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw notification stack (called by framework) |

**Usage:** Just call `notif_push()` — the framework handles update and draw.

---

## Tooltips

`#include "tooltips.h"`

Hover tooltips with fade-in delay.

```c
typedef struct {
    SDL_Rect triggerRect;
    const char *text;
    bool visible;
    float alpha;
    float timer;
} UITooltip;

typedef struct {
    UITooltip tooltips[32];
    int count;
} UITooltipManager;
```

| Function | Description |
|----------|-------------|
| `tooltip_manager_init(UITooltipManager *mgr)` | Initialize (reset count) |
| `tooltip_add(UITooltipManager *mgr, SDL_Rect trigger, const char *text)` | Register a tooltip region |
| `tooltip_update(UITooltipManager *mgr, SDL_Event *event, float dt)` | Check mouse hover (called by framework) |
| `tooltip_draw(UITooltipManager *mgr, UIContext *ui, SDL_Renderer *renderer, TTF_Font *font)` | Draw visible tooltips (called by framework) |

**Usage:** Re-register tooltips each frame (manager is reset per frame). The framework draws them after all widgets.

---

## Image

`#include "image.h"`

Static image and animated GIF loading/rendering.

```c
typedef struct {
    SDL_Texture *texture;
    int w, h;
} Image;

typedef struct {
    SDL_Texture *frames[GIF_MAX_FRAMES];
    int delays[GIF_MAX_FRAMES];
    int w, h;
    int frame_count;
    int current_frame;
    float elapsed;
    bool playing;
    bool loop;
} Gif;
```

### Image Functions

| Function | Description |
|----------|-------------|
| `image_load(Image *img, SDL_Renderer *renderer, const char *path)` | Load PNG/JPG. Returns true on success |
| `image_free(Image *img)` | Free texture |
| `image_draw(Image *img, SDL_Renderer *renderer, SDL_Rect dst)` | Draw to dst rect |

### GIF Functions

| Function | Description |
|----------|-------------|
| `gif_load(Gif *gif, SDL_Renderer *renderer, const char *path)` | Load animated GIF. Returns true on success |
| `gif_free(Gif *gif)` | Free all frames |
| `gif_update(Gif *gif, float dt)` | Advance frame (needs dt) |
| `gif_draw(Gif *gif, SDL_Renderer *renderer, SDL_Rect dst)` | Draw current frame |
| `gif_reset(Gif *gif)` | Reset to first frame |

---

## Image Widget

`#include "imageWidget.h"`

Convenience wrapper that auto-detects static vs GIF.

```c
typedef struct {
    Image image;
    Gif gif;
    bool is_gif;
    bool loaded;
    SDL_Rect rect;
} ImageWidget;
```

| Function | Description |
|----------|-------------|
| `image_widget_init(ImageWidget *iw, SDL_Renderer *renderer, const char *path)` | Load image or GIF from path |
| `image_widget_free(ImageWidget *iw)` | Free resources |
| `image_widget_update(ImageWidget *iw, float dt)` | Advance GIF if applicable |
| `image_widget_draw(ImageWidget *iw, SDL_Renderer *renderer, int x, int y, int w, int h)` | Draw at position |

---

## Orb

`#include "orb.h"`

Software-rendered 128×128 animated orb with GLSL-style FBM noise, ribbons, spherical lighting, and baked shadow. Rendered on a transparent background.

```c
#define ORB_RES 128

typedef struct {
    SDL_Texture *texture;
    Uint32 pixels[ORB_RES * ORB_RES];
    float time;
    float audio;
    bool dirty;
    bool visible;
    float dx, dy, dw, dh;
    SDL_Rect rect;
} Orb;
```

| Function | Description |
|----------|-------------|
| `orb_init(Orb *orb, SDL_Renderer *renderer)` | Initialize and create texture |
| `orb_free(Orb *orb)` | Free texture |
| `orb_update(Orb *orb, float dt)` | Re-render pixels (expensive, called every frame) |
| `orb_draw(Orb *orb, SDL_Renderer *renderer, SDL_Rect dst)` | Draw to dst rect |

**Note:** `orb_update()` runs the full FBM noise shader every frame. If performance is a concern, you can skip calls periodically.
