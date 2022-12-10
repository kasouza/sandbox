#include "systems/falling_sand.h"
#include "utils.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>

void box_falling_sand(struct BoxWorld *world)
{
    for (int x = 0; x < world->width; ++x)
    {
        for (int y = 0; y < world->height; ++y)
        {
            struct BoxTile tile = BOX_TILE_AT(x, y, world);
            struct BoxTileData data = BOX_TILE_DATA(tile);

            if (data.simulation_type == BOX_SOLID ||
                data.simulation_type == BOX_GAS)
                continue;
            else
            {
                // Check if tile bellow is available
                if (y > 0 &&
                    BOX_TILE_AT(x, y - 1, world).type == BOX_TILE_EMPTY)
                {
                    BOX_TILE_AT(x, y - 1, world) = tile;
                    BOX_TILE_AT(x, y, world) = box_empty_tile;
                    continue;
                }

                // Check if bottom left tile is available
                if (y > 0 && x > 0 &&
                    BOX_TILE_AT(x - 1, y - 1, world).type == BOX_TILE_EMPTY)
                {
                    BOX_TILE_AT(x - 1, y - 1, world) = tile;
                    BOX_TILE_AT(x, y, world) = box_empty_tile;
                }
                // Check if bottom right tile is available
                else if (y > 0 && x < world->width - 1 &&
                         BOX_TILE_AT(x + 1, y - 1, world).type ==
                             BOX_TILE_EMPTY)
                {
                    BOX_TILE_AT(x + 1, y - 1, world) = tile;
                    BOX_TILE_AT(x, y, world) = box_empty_tile;
                }

                else if (data.simulation_type == BOX_LIQUID)
                {
                    int option = (rand() > RAND_MAX / 2);
                    if (option == 0 && x > 0 &&
                        BOX_TILE_AT(x - 1, y, world).type == BOX_TILE_EMPTY)
                    {
                        BOX_TILE_AT(x - 1, y, world) = tile;
                        BOX_TILE_AT(x, y, world) = box_empty_tile;
                    }
                    else if (option == 1 && x < world->width - 1 &&
                             BOX_TILE_AT(x + 1, y, world).type ==
                                 BOX_TILE_EMPTY)
                    {
                        BOX_TILE_AT(x + 1, y, world) = tile;
                        BOX_TILE_AT(x, y, world) = box_empty_tile;
                    }
                }
            }
        }
    }
}
