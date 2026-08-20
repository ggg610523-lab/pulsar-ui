# Pulsar UI

A glassmorphism UI toolkit built on SDL2. Provides composable widgets, a unified application framework, animated backgrounds, and a 2x supersampled renderer with automatic downscaling.

## Dependencies

- SDL2
- SDL2_ttf
- SDL2_image

## Build

```bash
make clean && make
./liquid_glass
```

## Quick Start

```c
#include "pulsarUI.h"

int main(int argc, char *argv[])
{
    PulsarUI pui;
    pulsarui_init(&pui, "My App", 960, 720);

    UIButton btn;
    button_init(&btn, "Click Me");
    pulsarui_add_button(&pui, &btn, 400, 300, 160, 44);

    while (pulsarui_begin(&pui)) {

        if (btn.clicked)
            notif_push(&pui.notifications, "Clicked!");

        pulsarui_end(&pui);
        pulsarui_present(&pui);
    }

    pulsarui_shutdown(&pui);
    return 0;
}
```

Compile with:

```bash
gcc main.c -o app -Wall -Wextra -O2 \
    -I/usr/include/SDL2 -D_GNU_SOURCE -D_REENTRANT \
    -lSDL2 -lSDL2_ttf -lSDL2_image -lm
```

Or add all `.c` files to the Makefile.

## Architecture

- **2x supersampled render target** — everything is drawn at 2x resolution then downscaled to native, giving free anti-aliasing.
- **Responsive layout** — `UIContext` scales coordinates from a 960×720 design space. Call `ui_rect()` to convert design coords to scaled rects.
- **Dark/light theming** — `ui_theme(dark, light_color, dark_color)` returns the appropriate color for the current mode. Toggle at runtime via the built-in toggle switch.
- **Widget registration** — `pulsarui_add_*()` functions auto-wire event, layout, and draw callbacks. No manual vtable setup required.
- **Frame lifecycle** — `pulsarui_begin()` → handle events → draw widgets → `pulsarui_end()` (events, draws, downscale) → draw native-res overlays → `pulsarui_present()`.

## Widget Categories

| Category | Widgets |
|----------|---------|
| Basic | Panel, Button, PrimaryButton |
| Input | Slider, Radio, Checkbox, SearchBar, TextInput, Toggle |
| Layout | Tabs, MenuBar, Sidebar, Hamburger, DropdownMenu |
| Feedback | ProgressBar, Tooltips, Notifications, DialogBox |
| Media | Image, Orb |
| Background | Animated gradient + orbs |

## Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `PULSAR_MAX_WIDGETS` | 64 | Max widgets per PulsarUI instance |
| `SEARCHBAR_MAX` | 256 | Max search bar text length |
| `TEXTINPUT_MAX` | 512 | Max text input buffer size |
| `SIDEBAR_ITEMS` | 4 | Fixed sidebar item count |
| `DROPDOWN_MAX_ITEMS` | 16 | Max dropdown items |
| `TABS_MAX` | 8 | Max tab count |
| `MENUBAR_MAX_MENUS` | 8 | Max menu bar menus |
| `MENUBAR_MAX_ITEMS` | 16 | Max items per menu |
| `NOTIF_MAX` | 8 | Max active notifications |
| `NOTIF_TEXT_MAX` | 128 | Max notification text length |
| `GIF_MAX_FRAMES` | 256 | Max GIF frames |
| `ORB_RES` | 128 | Orb pixel resolution (128×128) |
