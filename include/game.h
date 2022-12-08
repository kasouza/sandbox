#ifndef KASOUZA_MINI_SURVIVAL_GAME_H
#define KASOUZA_MINI_SURVIVAL_GAME_H

#include <stdbool.h>

extern bool is_game_running;

// Is inlining these functions good? maybe
void update();
void render();

void init_game();
void quit_game();


#endif
