#include "systems/render.h"
#include "renderer.h"
#include "world.h"

void box_render(struct BoxWorld *world)
{
    for (int x = 0; x < world->width; ++x)
    {
        for (int y = 0; y < world->height; ++y)
        {
            struct BoxTile tile = BOX_TILE_AT(x, y, world);
            struct BoxTileData data = BOX_TILE_DATA(tile);
            box_render_draw_pixel(x, y, data.color.r, data.color.g,
                                  data.color.b);
        }
    }
}
