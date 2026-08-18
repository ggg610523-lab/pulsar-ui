#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

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

void notif_manager_init(
    NotificationManager *mgr
);

void notif_push(
    NotificationManager *mgr,
    const char *text
);

void notif_update(
    NotificationManager *mgr,
    float dt
);

void notif_draw(
    NotificationManager *mgr,
    UIContext *ui,
    SDL_Renderer *renderer,
    TTF_Font *font
);

#endif /* NOTIFICATIONS_H */
