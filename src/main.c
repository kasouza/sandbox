#include "main.h"
#include "renderer.h"
#include "utils.h"

#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool s_running = false;

void canvas_click_cb(int x, int y, enum BoxMouseButton button,
                     enum BoxInputAction action)
{
    if (button == BOX_MOUSE_BUTTON_1 && action == BOX_PRESS)
    {
        box_render_draw_pixel(x, y, 0, 0, 0);
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
    if (box_render_init((box_render_log_t)printf) != 0)
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
        box_render_present();
    }

    box_render_terminate();

    return 0;
}
