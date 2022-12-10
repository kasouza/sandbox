#include "main.h"
#include "renderer.h"
#include "systems/conways_game_of_life.h"
#include "utils.h"
#include "world.h"

#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_SIZE 256
static struct BoxWorld *s_world = NULL;

static bool s_running = false;
static bool s_pause = true;

void canvas_click_cb(int x, int y, enum BoxMouseButton button,
                     enum BoxInputAction action)
{

    unsigned char color = button == BOX_MOUSE_BUTTON_1 ? 255 : 0;
    if (action == BOX_PRESS)
    {
        int idx = BOX_INDEX(x, y, s_world->width);
        struct BoxTile *tile = &s_world->tiles[idx];
        tile->color.r = color;
        tile->color.g = color;
        tile->color.b = color;
        tile->color.a = color;
    }
}

void keyboard_cb(enum BoxKey key, enum BoxInputAction action)
{
    if (action == BOX_PRESS)
    {
        if (key == BOX_KEY_Q)
        {
            s_running = false;
        }

        if (key == BOX_KEY_SPACE)
        {
            s_pause = !s_pause;
        }
    }
}

// Render the world to the screen.
void render_system(const struct BoxWorld *world_in, struct BoxWorld *world_out)
{
    for (int i = 0; i < 256 / 1; ++i)
    {
        for (int j = 0; j < 256 / 1; ++j)
        {
            struct BoxTile tile = world_in->tiles[i + j * world_out->width];
            box_render_draw_pixel(i, j, tile.color.r, tile.color.g,
                                  tile.color.b);
        }
    }
}

int main()
{
    srand(time(NULL));
    if (box_render_init(WORLD_SIZE, WORLD_SIZE, (box_render_log_t)printf,
                        BOX_RENDER_UNLOCK_FPS) != 0)
    {
        err(BOX_RENDER_INIT_ERROR, "Could not initialize renderer.");
    }

    box_render_set_canvas_click_callback(canvas_click_cb);
    box_render_set_keyboard_callback(keyboard_cb);

    s_world = box_create_world(WORLD_SIZE, WORLD_SIZE);

    for (int i = 0; i < s_world->width; ++i)
    {
        for (int j = 0; j < s_world->height; ++j)
        {
            if (i % 7 != 0 && j % 3 != 0)
                continue;

            struct BoxTile *tile =
                &s_world->tiles[BOX_INDEX(i, j, s_world->width)];

            tile->color.r = 255;
            tile->color.g = 255;
            tile->color.b = 255;
            tile->color.a = 255;
        }
    }
    box_add_system(s_world, conways_game_of_life, 0);
    box_add_system(s_world, render_system, BOX_SYSTEM_PASSTHROUGH);

    s_running = true;
    bool removed = false;
    while (s_running)
    {
        box_render_clear();
        box_tick_world(s_world);
        if (s_pause && !removed)
        {
            box_remove_system(s_world, 0);
            removed = true;
        }
        else if (!s_pause && removed)
        {
            box_set_system(s_world, 0, conways_game_of_life, 0);
            removed = false;
        }
        box_render_present();
    }

    box_render_terminate();

    box_free_world(s_world);
    s_world = NULL;

    return 0;
}
