#include "world.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct BoxTileData box_tile_data_map[BOX_TILE_LAST] = {
    [BOX_TILE_EMPTY] = {0},

    [BOX_TILE_BARRIER] =
        {
            .color = {.r = 100, .g = 100, .b = 100, .a = 100},
            .mass = 9999.0,
            .simulation_type = BOX_SOLID,
        },
    [BOX_TILE_AIR] =
        {
            .color = {.r = 50, .g = 50, .b = 150, .a = 0},
            .mass = 0.1,
            .simulation_type = BOX_GAS,
        },

    [BOX_TILE_DIRT] =
        {
            .color = {.r = 125, .g = 60, .b = 40, .a = 255},
            .mass = 1.1,
            .simulation_type = BOX_SAND,
        },

    [BOX_TILE_IRON] =
        {
            .color = {.r = 100, .g = 100, .b = 100, .a = 100},
            .mass = 5.0,
            .simulation_type = BOX_SOLID,
        },

    [BOX_TILE_CGOL_CELL] =
        {
            .color = {.r = 0, .g = 255, .b = 0, .a = 255},
            .mass = 0.0,
            .simulation_type = BOX_SOLID,
        },

    [BOX_TILE_WATER] =
        {
            .color = {.r = 0, .g = 0, .b = 255, .a = 255},
            .mass = 1.0,
            .simulation_type = BOX_LIQUID,
        },
};

const struct BoxTile box_empty_tile = {
    .type = BOX_TILE_EMPTY,
};

struct BoxWorld *box_create_world(int width, int height)
{
    assert(width > 0 && height > 0 &&
           "World width and height must be greater than 0");

    struct BoxWorld *world = box_malloc(sizeof(struct BoxWorld));
    world->width = width;
    world->height = height;
    world->tiles = box_calloc(width * height, sizeof(struct BoxTile));

    return world;
}

void box_free_world(struct BoxWorld *world)
{
    assert(world != NULL);

    if (world->tiles)
    {
        free(world->tiles);
    }

    free(world);
}
