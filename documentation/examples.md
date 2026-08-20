# Examples

## Minimal App

```c
#include "pulsarUI.h"

int main(void)
{
    PulsarUI pui;
    pulsarui_init(&pui, "App", 960, 720);

    UIButton btn;
    button_init(&btn, "Hello");
    pulsarui_add_button(&pui, &btn, 400, 300, 160, 44);

    while (pulsarui_begin(&pui)) {
        if (btn.clicked)
            printf("clicked\n");

        pulsarui_end(&pui);
        pulsarui_present(&pui);
    }

    pulsarui_shutdown(&pui);
    return 0;
}
```

---

## Slider + Progress Bar

```c
UISlider slider;
slider_init(&slider, 0.5f);
pulsarui_add_slider(&pui, &slider, 100, 200, 300);

UIProgressBar bar;
progress_bar_init(&bar, 0.5f);
pulsarui_add_progress_bar(&pui, &bar, 100, 240, 300, 12);

// Auto-sync slider to progress bar
pulsarui_link_progress(&pui, &slider, &bar);

// Or read slider.value manually each frame
while (pulsarui_begin(&pui)) {
    printf("value: %.2f\n", slider.value);
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Radio Group (Manual Mutual Exclusion)

```c
UIRadio radios[3];
radio_init(&radios[0], "Option A", true);
radio_init(&radios[1], "Option B", false);
radio_init(&radios[2], "Option C", false);

pulsarui_add_radio(&pui, &radios[0], 100, 200);
pulsarui_add_radio(&pui, &radios[1], 100, 240);
pulsarui_add_radio(&pui, &radios[2], 100, 280);

while (pulsarui_begin(&pui)) {
    // Check which radio was just selected
    for (int i = 0; i < 3; i++) {
        if (radios[i].selected) {
            // Deselect the others
            for (int j = 0; j < 3; j++) {
                if (j != i)
                    radios[j].selected = false;
            }
            printf("Selected: %d\n", i);
        }
    }
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Dark Mode Toggle

```c
UIToggle toggle;
toggle_init(&toggle, false);
pulsarui_add_toggle(&pui, &toggle, 860, 78, 64, 32);

// The framework auto-wires this toggle to trigger
// the dark mode fade transition. Just register it.
// pui.ui.dark is updated automatically during the
// fade-through-black transition.

// Check current mode:
while (pulsarui_begin(&pui)) {
    if (pui.ui.dark)
        printf("dark mode\n");
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Menu Bar Actions

```c
UIMenuBar menubar;
menubar_init(&menubar);
menubar_add_menu(&menubar, "File");
menubar_add_item(&menubar, 0, "New");
menubar_add_item(&menubar, 0, "Open");
menubar_add_item(&menubar, 0, "Save");
menubar_add_menu(&menubar, "Edit");
menubar_add_item(&menubar, 1, "Undo");
menubar_add_item(&menubar, 1, "Copy");
pulsarui_add_menubar(&pui, &menubar, 200, 10, 500);

while (pulsarui_begin(&pui)) {
    int menuIdx, itemIdx;
    if (menubar_selected(&menubar, &menuIdx, &itemIdx)) {
        printf("Menu %d, Item %d\n", menuIdx, itemIdx);
    }
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Sidebar + Hamburger

```c
UIHamburger hamburger;
hamburger_init(&hamburger);
pulsarui_add_hamburger(&pui, &hamburger, 15, 15, 40);

UISidebar sidebar;
sidebar_init(&sidebar);
pulsarui_add_sidebar(&pui, &sidebar, 0, 0, 260, 700);

// Wire them together
pulsarui_link_sidebar(&pui, &sidebar, &hamburger);

// sidebar.open is toggled automatically when
// hamburger is clicked. Use sidebar.anim for
// smooth layout offset:
while (pulsarui_begin(&pui)) {
    float offset = ui_ease_out_cubic(sidebar.anim) * 280.0f;
    // Shift content right by offset
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Notifications

```c
UIButton notifBtn;
button_init(&notifBtn, "Show Notification");
pulsarui_add_button(&pui, &notifBtn, 100, 100, 200, 44);

while (pulsarui_begin(&pui)) {
    if (notifBtn.clicked)
        notif_push(&pui.notifications, "Hello world!");

    // Notifications are updated and drawn by the
    // framework automatically.
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Tooltips

```c
UIButton btn;
button_init(&btn, "Hover me");
pulsarui_add_button(&pui, &btn, 100, 100, 160, 44);

while (pulsarui_begin(&pui)) {
    // Register tooltips each frame (manager resets)
    tooltip_manager_init(&pui.tooltips);
    tooltip_add(&pui.tooltips, btn.rect, "Click this button");

    // Framework draws tooltips after all widgets.
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Image Widget

```c
ImageWidget img;
image_widget_init(&img, pui.renderer, "photo.png");
pulsarui_add_image(&pui, &img, 100, 100, 300, 200);

// For GIFs:
ImageWidget gif;
image_widget_init(&gif, pui.renderer, "anim.gif");
pulsarui_add_image(&pui, &gif, 100, 100, 300, 200);

while (pulsarui_begin(&pui)) {
    // GIF frame advancement is handled by the framework
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}

image_widget_free(&img);
image_widget_free(&gif);
```

---

## Text Input with Submit

```c
UITextInput input;
textinput_init(&input, 20);
pulsarui_add_textinput(&pui, &input, 100, 100, 400, 40);

while (pulsarui_begin(&pui)) {
    if (textinput_submit(&input)) {
        printf("Submitted: %s\n", textinput_get(&input));
        textinput_clear(&input);
    }
    pulsarui_end(&pui);
    pulsarui_present(&pui);
}
```

---

## Dialog Box

```c
DialogBox dialog;
dialog_box_init(&dialog, pui.font);
pulsarui_add_dialog(&pui, &dialog);

UIButton openBtn;
button_init(&openBtn, "Open Dialog");
pulsarui_add_button(&pui, &openBtn, 100, 100, 160, 44);

while (pulsarui_begin(&pui)) {
    if (openBtn.clicked)
        dialog_box_open(&dialog);

    pulsarui_end(&pui);
    pulsarui_present(&pui);

    // Dialog has its own window — draw after present
    dialog_box_draw(&dialog, pui.ui.dark, pui.dt);
}

dialog_box_close(&dialog);
pulsarui_shutdown(&pui);
```

---

## Custom Drawing (Native Resolution)

Draw text or shapes at native resolution between `pulsarui_end()` and `pulsarui_present()`:

```c
while (pulsarui_begin(&pui)) {
    // ... widget logic ...

    pulsarui_end(&pui);

    // Native-res drawing (after 2x downscale)
    UIContext *ui = &pui.ui;

    ui_text_shadow(
        pui.renderer,
        pui.titleFont,
        "My Title",
        (int)(55 * ui->scale),
        (int)(60 * ui->scale),
        ui_theme(ui->dark,
            (UIColor){25, 32, 50, 255},
            (UIColor){220, 225, 240, 255}),
        ui_theme(ui->dark,
            (UIColor){255, 255, 255, 45},
            (UIColor){0, 0, 0, 60}),
        1);

    pulsarui_present(&pui);
}
```

---

## Theming Widgets

Use `ui_theme()` to adapt colors to dark/light mode:

```c
UIColor textColor = ui_theme(ui->dark,
    (UIColor){25, 35, 55, 255},    // light mode
    (UIColor){220, 225, 240, 255}  // dark mode
);

UIColor bgColor = ui_theme(ui->dark,
    (UIColor){240, 242, 248, 200},
    (UIColor){30, 35, 50, 200}
);
```

---

## Glass Panel

```c
SDL_Rect panel = ui_rect(&pui.ui, 50, 50, 300, 200);

ui_glass(
    pui.renderer,
    panel,
    (int)(24 * pui.ui.scale),  // radius
    false,                      // active (outline highlight)
    pui.ui.dark                 // theme
);
```
