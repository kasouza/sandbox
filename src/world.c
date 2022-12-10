#include "world.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BoxWorld *box_create_world(int width, int height)
{
    assert(width > 0 && height > 0 &&
           "World width and height must be greater than 0");

    struct BoxWorld *world = box_malloc(sizeof(struct BoxWorld));
    world->width = width;
    world->height = height;
    world->tiles = box_calloc(width * height, sizeof(struct BoxTile));

    for (int i = 0; i < BOX_MAX_SYSTEMS; ++i)
    {
        world->systems[i].run = NULL;
        world->systems[i].flags = 0;
    }

    world->system_count = 0;

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

int box_add_system(struct BoxWorld *world, box_system_t system,
                   int system_flags)
{
    assert(world != NULL);
    assert(system != NULL);
    assert(world->system_count < BOX_MAX_SYSTEMS &&
           "Can't add more systems to world.");

    int system_id = -1;

    // Check for an avaiable space
    for (int i = 0; i < BOX_MAX_SYSTEMS; ++i)
    {
        if (world->systems[i].run == NULL)
        {
            system_id = i;
            break;
        }
    }

    assert(system_id != -1 && "Can't add more systems to world.");

    box_set_system(world, system_id, system, system_flags);

    return system_id;
}

void box_set_system(struct BoxWorld *world, int system_id, box_system_t system,
                    int system_flags)
{
    assert(world != NULL);
    assert(system != NULL);
    assert(system_id >= 0 && system_id < BOX_MAX_SYSTEMS);

    if (world->systems[system_id].run == NULL)
        world->system_count++;

    world->systems[system_id].run = system;
    world->systems[system_id].flags = system_flags;
}

void box_remove_system(struct BoxWorld *world, int system_id)
{
    assert(world != NULL);
    assert(system_id >= 0 && system_id < BOX_MAX_SYSTEMS);

    world->systems[system_id].run = NULL;
    world->systems[system_id].flags = 0;
    world->system_count--;
}

void box_tick_world(struct BoxWorld *world)
{
    assert(world != NULL);
    assert(world->tiles != NULL);

    // This should be static
    struct BoxWorld *output_world =
        box_create_world(world->width, world->height);

    // This loops through the systems skipping empty spot in the world->systems
    // array
    int ticked_system_count = 0;
    int system_idx = 0;
    while (ticked_system_count < world->system_count)
    {
        box_system_t system = world->systems[system_idx].run;
        int flags = world->systems[system_idx].flags;
        if (system)
        {
            if (flags & BOX_SYSTEM_EMPTY_OUTPUT_BUFFER)
            {
                memset(output_world->tiles, 0,
                       output_world->width * output_world->height *
                           sizeof(struct BoxTile));
            }

            system(world, output_world);

            // Do not swap the buffers if the system has no output (like a
            // rendering system)
            if (!(flags & BOX_SYSTEM_PASSTHROUGH))
            {
                struct BoxTile *temp_tiles = world->tiles;
                world->tiles = output_world->tiles;
                output_world->tiles = temp_tiles;
            }

            ticked_system_count++;
        }

        system_idx++;
    }

    box_free_world(output_world);
    output_world = NULL;
}
