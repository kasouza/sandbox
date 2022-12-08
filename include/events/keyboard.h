#ifndef KASOUZA_MINI_SURVIVAL_EVENTS_KEYBOARD_H
#define KASOUZA_MINI_SURVIVAL_EVENTS_KEYBOARD_H

#include <SDL2/SDL.h>

#include <stdbool.h>

enum Key
{
	key_w,
	key_a,
	key_s,
	key_d,
};

extern bool pressed_keys[4];

void handle_keyboard_events(SDL_Event* event);

#endif
