#include "systems/falling_sand.h"
#include "utils.h"
#include "world.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool *s_updated_tiles = NULL;
static int s_updated_tiles_width = -1;
static int s_updated_tiles_height = -1;

void box_falling_sand(struct BoxWorld *world)
{
    if (!s_updated_tiles || world->width != s_updated_tiles_width ||
        world->height != s_updated_tiles_height)
    {
        if (s_updated_tiles != NULL)
        {
            // Should use realloc here instead of freeing and allocating again,
            // but shouldn't be a huge concern, because most of the time all the
            // calls to this function work with worlds of the same size
            free(s_updated_tiles);
        }

        s_updated_tiles =
            box_calloc(world->width * world->height, sizeof(bool));
        s_updated_tiles_width = world->width;
        s_updated_tiles_height = world->height;
    }

    for (int x = 0; x < world->width; ++x)
    {
        for (int y = 0; y < world->height; ++y)
        {
            if (s_updated_tiles[BOX_TILE_INDEX(x, y, world)])
                continue;

            struct BoxTile tile = BOX_TILE_AT(x, y, world);
            struct BoxTileData data = BOX_TILE_DATA(tile);

            int y_offset = -1;
            if (data.simulation_type == BOX_GAS)
            {
                y_offset = 1;
            }

            bool y_within_bounds = ((y > 0) && (y < (world->height - 1)));

            if (data.simulation_type == BOX_SOLID)
                continue;
            else
            {
                // Check if tile directly bellow/above is available
                /* if (y_within_bounds && */
                /* BOX_TILE_AT(x, y + y_offset, world).type == BOX_TILE_EMPTY)
                 */
                /* { */
                /* BOX_TILE_AT(x, y + y_offset, world) = tile; */
                /* s_updated_tiles[BOX_TILE_INDEX(x, y + y_offset, world)] = */
                /* true; */
                /* BOX_TILE_AT(x, y, world) = box_empty_tile; */
                /* continue; */
                /* } */

                // Wether this tile should try to go to the bottom/top left or
                // bottom/top right
                int option = (rand() > RAND_MAX / 2);

                // Check if bottom left tile is available
                if (option == 0 && y_within_bounds && x > 0 &&
                    BOX_TILE_AT(x - 1, y + y_offset, world).type ==
                        BOX_TILE_EMPTY)
                {
                    BOX_TILE_AT(x - 1, y + y_offset, world) = tile;
                    s_updated_tiles[BOX_TILE_INDEX(x - 1, y + y_offset,
                                                   world)] = true;
                    BOX_TILE_AT(x, y, world) = box_empty_tile;
                }
                // Check if bottom right tile is available
                else if (option == 1 && y_within_bounds &&
                         x < world->width - 1 &&
                         BOX_TILE_AT(x + 1, y + y_offset, world).type ==
                             BOX_TILE_EMPTY)
                {
                    BOX_TILE_AT(x + 1, y + y_offset, world) = tile;
                    s_updated_tiles[BOX_TILE_INDEX(x + 1, y + y_offset,
                                                   world)] = true;
                    BOX_TILE_AT(x, y, world) = box_empty_tile;
                }
                else if (data.simulation_type == BOX_LIQUID ||
                         data.simulation_type == BOX_GAS)
                {
                    // Wether this tile should try to go to the left or right
                    int option = (rand() > RAND_MAX / 2);

                    if (option == 0 && x > 0 &&
                        BOX_TILE_AT(x - 1, y, world).type == BOX_TILE_EMPTY)
                    {
                        BOX_TILE_AT(x - 1, y, world) = tile;
                        s_updated_tiles[BOX_TILE_INDEX(x - 1, y, world)] = true;
                        BOX_TILE_AT(x, y, world) = box_empty_tile;
                    }
                    else if (option == 1 && x < world->width - 1 &&
                             BOX_TILE_AT(x + 1, y, world).type ==
                                 BOX_TILE_EMPTY)
                    {
                        BOX_TILE_AT(x + 1, y, world) = tile;
                        s_updated_tiles[BOX_TILE_INDEX(x + 1, y, world)] = true;
                        BOX_TILE_AT(x, y, world) = box_empty_tile;
                    }
                }
                else if (y_within_bounds)
                {
                    struct BoxTile other = BOX_TILE_AT(x, y + y_offset, world);
                    struct BoxTileData other_data = BOX_TILE_DATA(other);

                    if ((other_data.simulation_type != BOX_SOLID ||
                         other.type == BOX_TILE_EMPTY) &&
                        other.type != tile.type &&
                        ((y_offset > 0 && other_data.mass > data.mass) ||
                         (y_offset < 0 && other_data.mass < data.mass)))
                    {
                        BOX_TILE_AT(x, y, world) = other;
                        BOX_TILE_AT(x, y + y_offset, world) = tile;
                        s_updated_tiles[BOX_TILE_INDEX(x, y, world)] = true;
                        s_updated_tiles[BOX_TILE_INDEX(x, y, world)] = true;
                        continue;
                    }
                }
            }
        }
    }

    memset(s_updated_tiles, 0,
           s_updated_tiles_width * s_updated_tiles_height * sizeof(bool));
}

void box_terminate_falling_sand()
{
    if (s_updated_tiles)
    {
        free(s_updated_tiles);
        s_updated_tiles = NULL;
        s_updated_tiles_width = -1;
        s_updated_tiles_height = -1;
    }
}
