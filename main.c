
#include "pulsarUI.h"

int main(void)
{
    PulsarUI pui;

    if (
        pulsarui_init(
            &pui,
            "Pulsar UI",
            1100,
            700
        ) != 0
    ) {
        return 1;
    }

    /* -------------------------------------------------- */
    /*  WIDGETS                                           */
    /* -------------------------------------------------- */

    UIPanel panel;
    panel_init(&panel, "Material controls");

    UIButton button;
    button_init(&button, "Glass Button");

    UIPrimaryButton primaryButton;
    primary_button_init(&primaryButton, "OK");

    UISlider slider;
    slider_init(&slider, 0.58f);

    UIProgressBar progressbar;
    progress_bar_init(&progressbar, 0.58f);

    UIRadio radios[3];
    radio_init(&radios[0], "Liquid Glass", true);
    radio_init(&radios[1], "Frosted", false);
    radio_init(&radios[2], "Solid", false);

    UICheckBox checkbox;
    checkbox_init(&checkbox, "Enable test", true);

    UITabs tabs;
    tabs_init(&tabs);
    tabs_add(&tabs, "General");
    tabs_add(&tabs, "Appearance");
    tabs_add(&tabs, "Advanced");

    UISearchBar searchbar;
    searchbar_init(&searchbar);

    UIDropDown dropdown;
    dropdown_init(&dropdown);
    dropdown_add_item(&dropdown, "Option 1");
    dropdown_add_item(&dropdown, "Option 2");
    dropdown_add_item(&dropdown, "Option 3");
    dropdown_add_item(&dropdown, "Option 4");

    UIHamburger hamburger;
    hamburger_init(&hamburger);

    UIMenuBar menubar;
    menubar_init(&menubar);
    menubar_add_menu(&menubar, "File");
    menubar_add_item(&menubar, 0, "New");
    menubar_add_item(&menubar, 0, "Open");
    menubar_add_item(&menubar, 0, "Save");
    menubar_add_item(&menubar, 0, "Exit");
    menubar_add_menu(&menubar, "Edit");
    menubar_add_item(&menubar, 1, "Undo");
    menubar_add_item(&menubar, 1, "Redo");
    menubar_add_item(&menubar, 1, "Copy");
    menubar_add_item(&menubar, 1, "Paste");
    menubar_add_menu(&menubar, "Help");
    menubar_add_item(&menubar, 2, "About");
    menubar_add_item(
        &menubar, 2, "Documentation"
    );

    UISidebar sidebar;
    sidebar_init(&sidebar);

    DialogBox dialog;
    dialog_box_init(&dialog, pui.font);

    UIButton notifButton;
    button_init(&notifButton, "Notify");

    Orb orb;
    orb_init(&orb, pui.renderer);

    UIToggle themeToggle;
    toggle_init(&themeToggle, false);

    /* -------------------------------------------------- */
    /*  REGISTER (toolkit handles layout + events + draw) */
    /* -------------------------------------------------- */

    pulsarui_add_panel(
        &pui, &panel, 55, 190, 480, 385);

    pulsarui_add_tabs(
        &pui, &tabs, 570, 410, 450);

    pulsarui_add_button(
        &pui, &button, 90, 480, 190, 55);

    pulsarui_add_primary_button(
        &pui, &primaryButton,
        300, 480, 190, 55);

    pulsarui_add_slider(
        &pui, &slider, 310, 280, 190);

    pulsarui_add_progress_bar(
        &pui, &progressbar,
        570, 490, 450, 12);

    pulsarui_add_radio(
        &pui, &radios[0], 90, 280);

    pulsarui_add_radio(
        &pui, &radios[1], 90, 325);

    pulsarui_add_radio(
        &pui, &radios[2], 90, 370);

    pulsarui_add_checkbox(
        &pui, &checkbox, 310, 350);

    pulsarui_add_searchbar(
        &pui, &searchbar,
        55, 590, 480, 45);

    pulsarui_add_dropdown(
        &pui, &dropdown,
        310, 420, 190);

    pulsarui_add_hamburger(
        &pui, &hamburger, 15, 15, 40);

    pulsarui_add_menubar(
        &pui, &menubar, 570, 15, 450);

    pulsarui_add_sidebar(
        &pui, &sidebar,
        0, 0, 260, 700);

    pulsarui_add_dialog(&pui, &dialog);

    pulsarui_add_button(
        &pui, &notifButton,
        570, 520, 120, 45);

    pulsarui_add_orb(
        &pui, &orb,
        640, 530, 200, 200);

    pulsarui_add_toggle(
        &pui, &themeToggle,
        860, 78, 64, 32);

    pulsarui_link_sidebar(
        &pui, &sidebar, &hamburger);

    pulsarui_link_progress(
        &pui, &slider, &progressbar);

    /* -------------------------------------------------- */
    /*  MAIN LOOP                                         */
    /* -------------------------------------------------- */

    while (pulsarui_begin(&pui)) {

        /* ---- tooltips ---- */

        tooltip_manager_init(
            &pui.tooltips
        );

        tooltip_add(
            &pui.tooltips,
            button.rect,
            "Click to open dialog"
        );

        tooltip_add(
            &pui.tooltips,
            primaryButton.rect,
            "Primary action"
        );

        tooltip_add(
            &pui.tooltips,
            slider.rect,
            "Adjust material intensity"
        );

        tooltip_add(
            &pui.tooltips,
            checkbox.rect,
            "Toggle reflections"
        );

        tooltip_add(
            &pui.tooltips,
            hamburger.rect,
            "Open menu"
        );

        /* ---- actions ---- */

        if (button.clicked) {
            dialog_box_open(&dialog);
        }

        if (primaryButton.clicked) {
            printf("Primary button clicked\n");
        }

        if (notifButton.clicked) {
            notif_push(
                &pui.notifications,
                "Hello from notification!"
            );
        }

        pulsarui_end(&pui);

        /* ---- native-res text ---- */

        UIContext *ui = &pui.ui;

        int width;
        SDL_GetWindowSize(
            pui.window, &width, NULL
        );

        float titleX =
            55.0f +
            ui_ease_out_cubic(
                sidebar.anim
            ) * 280.0f;

        ui_text_shadow(
            pui.renderer,
            pui.titleFont,
            "Pulsar UI",
            (int)(titleX * ui->scale),
            (int)(60 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){25, 32, 50, 255},
                (UIColor){220, 225, 240, 255}),
            ui_theme(ui->dark,
                (UIColor){255, 255, 255, 45},
                (UIColor){0, 0, 0, 60}),
            1);

        ui_text_shadow(
            pui.renderer,
            pui.font,
            "C / SDL2 scalable interface",
            (int)((titleX + 3) * ui->scale),
            (int)(112 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255}),
            ui_theme(ui->dark,
                (UIColor){255, 255, 255, 30},
                (UIColor){0, 0, 0, 40}),
            1);

        float rightX =
            width < 800 ? 55.0f : 570.0f;

        SDL_Rect info =
            ui_rect(
                ui,
                rightX,
                width < 800 ? 600 : 190,
                450, 190
            );

        ui_glass(
            pui.renderer,
            info,
            (int)(24 * ui->scale),
            false,
            ui->dark
        );

        ui_text_shadow(
            pui.renderer,
            pui.font,
            "Optical material",
            info.x + (int)(25 * ui->scale),
            info.y + (int)(22 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){25, 32, 50, 255},
                (UIColor){220, 225, 240, 255}),
            ui_theme(ui->dark,
                (UIColor){255, 255, 255, 35},
                (UIColor){0, 0, 0, 50}),
            1);

        ui_text(
            pui.renderer, pui.font,
            "Crisp translucency",
            info.x + (int)(25 * ui->scale),
            info.y + (int)(65 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            pui.renderer, pui.font,
            "Edge lighting",
            info.x + (int)(25 * ui->scale),
            info.y + (int)(98 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            pui.renderer, pui.font,
            "Simulated lensing",
            info.x + (int)(220 * ui->scale),
            info.y + (int)(65 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        ui_text(
            pui.renderer, pui.font,
            "Responsive layout",
            info.x + (int)(220 * ui->scale),
            info.y + (int)(98 * ui->scale),
            ui_theme(ui->dark,
                (UIColor){65, 75, 95, 255},
                (UIColor){155, 165, 185, 255})
        );

        pulsarui_present(&pui);

        dialog_box_draw(
            &dialog,
            pui.ui.dark,
            pui.dt
        );
    }

    dialog_box_close(&dialog);
    pulsarui_shutdown(&pui);

    return 0;
}
