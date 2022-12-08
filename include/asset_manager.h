#ifndef KASOUZA_MINI_SURVIVAL_ASSET_MANAGER_H
#define KASOUZA_MINI_SURVIVAL_ASSET_MANAGER_H

#include <SDL2/SDL.h>

#define MAX_TEXTURES 32

void init_asset_manager();
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);
void free_textures();

#endif
