#include "main.h"
#include "renderer.h"
#include "utils.h"

#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_SIZE 256

static int buffer[WORLD_SIZE * WORLD_SIZE * 4] = {0};

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

int main()
{
    srand(time(NULL));
    if (box_render_init(WORLD_SIZE, WORLD_SIZE, (box_render_log_t)printf) != 0)
    {
        err(BOX_RENDER_INIT_ERROR, "Could not initialize renderer.");
    }

    for (int i = 0; i < 256 / 1; ++i)
    {
        for (int j = 0; j < 256 / 1; ++j)
        {
            int r = rand() % UINT8_MAX;
            int g = rand() % UINT8_MAX;
            int b = rand() % UINT8_MAX;
            box_render_draw_pixel(i, j, r, g, b);
        }
    }

    box_render_set_canvas_click_callback(canvas_click_cb);
    box_render_set_keyboard_callback(keyboard_cb);

    s_running = true;
    while (s_running)
    {
        box_render_clear();
        for (int x = 0; x < WORLD_SIZE; ++x)
        {
            for (int y = 0; y < WORLD_SIZE; ++y)
            {
                int idx = (x + y * WORLD_SIZE) * 4;
                int red = buffer[idx];
                int green = buffer[idx];
                int blue = buffer[idx];
                box_render_draw_pixel(x, y, red, green, blue);
            }
        }
        box_render_present();
    }

    box_render_terminate();

    return 0;
}
