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
           (BOX_TILE_AT(x - 1, y - 1, world_in).type == BOX_TILE_CGOL_CELL);

    // North
    sum +=
        (y > 0) && (BOX_TILE_AT(x, y - 1, world_in).type == BOX_TILE_CGOL_CELL);

    // North East
    sum += ((x < w) && (y > 0)) &&
           (BOX_TILE_AT(x + 1, y - 1, world_in).type == BOX_TILE_CGOL_CELL);

    // West
    sum +=
        (x > 0) && (BOX_TILE_AT(x - 1, y, world_in).type == BOX_TILE_CGOL_CELL);

    // East
    sum +=
        (x < w) && (BOX_TILE_AT(x + 1, y, world_in).type == BOX_TILE_CGOL_CELL);

    // South West
    sum += ((x > 0) && (y < h)) &&
           (BOX_TILE_AT(x - 1, y + 1, world_in).type == BOX_TILE_CGOL_CELL);

    // South
    sum +=
        (y < h) && (BOX_TILE_AT(x, y + 1, world_in).type == BOX_TILE_CGOL_CELL);

    // South East
    sum += ((x < w) && (y < h)) &&
           (BOX_TILE_AT(x + 1, y + 1, world_in).type == BOX_TILE_CGOL_CELL);

    return sum;
}

void box_conways_game_of_life(struct BoxWorld *world)
{
    struct BoxWorld *world_out = box_create_world(world->width, world->height);

    for (int i = 0; i < world->width; ++i)
    {
        for (int j = 0; j < world->height; ++j)
        {
            {
                int idx = BOX_TILE_INDEX(i, j, world);

                struct BoxTile tile = world->tiles[idx];
                struct BoxTile *next_tile = &world_out->tiles[idx];

                if (tile.type != BOX_TILE_EMPTY &&
                    tile.type != BOX_TILE_CGOL_CELL)
                {
                    *next_tile = tile;
                }

                int neighbor_count = count_neighbors(world, i, j);
                if ((tile.type == BOX_TILE_EMPTY && neighbor_count == 3) ||
                    (tile.type == BOX_TILE_CGOL_CELL &&
                     (neighbor_count == 2 || neighbor_count == 3)))
                {
                    next_tile->type = BOX_TILE_CGOL_CELL;
                }
                else if (tile.type == BOX_TILE_CGOL_CELL)
                {
                    next_tile->type = BOX_TILE_EMPTY;
                }
            }
        }
    }

    struct BoxTile *temp_tiles = world->tiles;
    world->tiles = world_out->tiles;
    world_out->tiles = temp_tiles;

    box_free_world(world_out);
}
