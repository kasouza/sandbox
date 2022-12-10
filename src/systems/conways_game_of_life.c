#include "systems/conways_game_of_life.h"
#include "utils.h"
#include "world.h"

inline static int count_neighbors(const struct BoxWorld *world_in, int x, int y)
{
    int sum = 0;
    int w = world_in->width;
    int h = world_in->height;

    // North West
    sum += ((x > 0) && (y > 0)) &&
           (world_in->tiles[BOX_INDEX(x - 1, y - 1, w)].color.r == 255);

    // North
    sum += (y > 0) && (world_in->tiles[BOX_INDEX(x, y - 1, w)].color.r == 255);

    // North East
    sum += ((x < w) && (y > 0)) &&
           (world_in->tiles[BOX_INDEX(x + 1, y - 1, w)].color.r == 255);

    // West
    sum += (x > 0) && (world_in->tiles[BOX_INDEX(x - 1, y, w)].color.r == 255);

    // East
    sum += (x < w) && (world_in->tiles[BOX_INDEX(x + 1, y, w)].color.r == 255);

    // South West
    sum += ((x > 0) && (y < h)) &&
           (world_in->tiles[BOX_INDEX(x - 1, y + 1, w)].color.r == 255);

    // South
    sum += (y < h) && (world_in->tiles[BOX_INDEX(x, y + 1, w)].color.r == 255);

    // South East
    sum += ((x < w) && (y < h)) &&
           (world_in->tiles[BOX_INDEX(x + 1, y + 1, w)].color.r == 255);

    return sum;
}

void conways_game_of_life(const struct BoxWorld *world_in,
                          struct BoxWorld *world_out)
{
    for (int i = 0; i < world_in->width; ++i)
    {
        for (int j = 0; j < world_in->height; ++j)
        {
            int neighbor_count = count_neighbors(world_in, i, j);
            int idx = BOX_INDEX(i, j, world_in->width);

            struct BoxTile tile = world_in->tiles[idx];
            struct BoxTile *next_tile = &world_out->tiles[idx];

            if ((tile.color.r == 0 && neighbor_count == 3) ||
                (tile.color.r == 255 &&
                 (neighbor_count == 2 || neighbor_count == 3)))
            {
                next_tile->color.r = 255;
                next_tile->color.g = 255;
                next_tile->color.b = 255;
                next_tile->color.a = 255;
            }
            else
            {
                next_tile->color.r = 0;
                next_tile->color.g = 0;
                next_tile->color.b = 0;
                next_tile->color.a = 0;
            }
        }
    }
}
