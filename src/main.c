#include "main.h"
#include "renderer.h"
#include "systems/conways_game_of_life.h"
#include "systems/falling_sand.h"
#include "systems/render.h"
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
static bool s_pause = false;
static enum BoxTileType s_selected_tile = BOX_TILE_DIRT;

void canvas_click_cb(int x, int y, enum BoxMouseButton button,
                     enum BoxInputAction action)
{
    if (action == BOX_PRESS)
    {
        int type =
            button == BOX_MOUSE_BUTTON_1 ? s_selected_tile : BOX_TILE_EMPTY;
        BOX_TILE_AT(x, y, s_world).type = type;
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

        for (int i = 0; i < BOX_MIN(9, BOX_TILE_LAST); ++i)
        {
            if (key == BOX_KEY_0 + i)
            {
                s_selected_tile = i;
                break;
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    if (box_render_init(WORLD_SIZE, WORLD_SIZE, (box_render_log_t)printf,
                        BOX_RENDER_UNLOCK_FPS) != 0)
        err(BOX_RENDER_INIT_ERROR, "Could not initialize renderer.");

    box_render_set_canvas_click_callback(canvas_click_cb);
    box_render_set_keyboard_callback(keyboard_cb);

    s_world = box_create_world(WORLD_SIZE, WORLD_SIZE);
    s_running = true;

    while (s_running)
    {
        // Update the world
        if (!s_pause)
        {
            box_falling_sand(s_world);
            box_conways_game_of_life(s_world);
        }

        // Rendering
        box_render_clear();
        box_render(s_world);
        box_render_present();
    }

    box_render_terminate();

    box_free_world(s_world);
    s_world = NULL;

    return 0;
}
