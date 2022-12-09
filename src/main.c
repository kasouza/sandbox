#include "main.h"
#include "renderer.h"
#include "utils.h"
#include "world.h"

#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_SIZE 256

static int buffer[WORLD_SIZE * WORLD_SIZE * 4] = {0};
static struct BoxWorld *s_world = NULL;

static bool s_running = false;

void canvas_click_cb(int x, int y, enum BoxMouseButton button,
                     enum BoxInputAction action)
{
    if (button == BOX_MOUSE_BUTTON_1 && action == BOX_PRESS)
    {
        int idx = (x + y * WORLD_SIZE) * 4;
        buffer[idx] = 255;
        buffer[idx + 1] = 0;
        buffer[idx + 2] = 0;
        buffer[idx + 3] = 255;
    }
}

void keyboard_cb(enum BoxKey key, enum BoxInputAction action)
{
    if (key == BOX_KEY_Q)
    {
        s_running = false;
    }
}

// Copy the input world into the output world.
void copy_system(const struct BoxWorld *world_in, struct BoxWorld *world_out)
{
    for (int i = 0; i < 256 / 1; ++i)
    {
        for (int j = 0; j < 256 / 1; ++j)
        {
            int idx = BOX_INDEX(i, j, world_in->width);
            world_out->tiles[idx] = world_in->tiles[idx];
        }
    }
}

// Random color for each pixel/tile
void random_system(const struct BoxWorld *world_in, struct BoxWorld *world_out)
{
    for (int i = 0; i < 256 / 1; ++i)
    {
        for (int j = 0; j < 256 / 1; ++j)
        {
            int number = rand() % UINT8_MAX;
            /* int number = 3; */
            struct BoxTile *tile =
                &world_out->tiles[BOX_INDEX(i, j, world_in->width)];
            tile->color.r = number;
            tile->color.g = number;
            tile->color.b = number;
            tile->color.a = 255;
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
    if (box_render_init(WORLD_SIZE, WORLD_SIZE, (box_render_log_t)printf) != 0)
    {
        err(BOX_RENDER_INIT_ERROR, "Could not initialize renderer.");
    }

    box_render_set_canvas_click_callback(canvas_click_cb);
    box_render_set_keyboard_callback(keyboard_cb);

    s_world = box_create_world(WORLD_SIZE, WORLD_SIZE);

    box_add_system(s_world, random_system, 0);
    box_add_system(s_world, render_system, BOX_SYSTEM_PASSTHROUGH);

    s_running = true;
    while (s_running)
    {
        box_render_clear();
        box_tick_world(s_world);
        box_render_present();
    }

    box_render_terminate();

    box_free_world(s_world);
    s_world = NULL;

    return 0;
}
