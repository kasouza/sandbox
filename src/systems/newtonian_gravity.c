#include "systems/newtonian_gravity.h"
#include "utils.h"
#include "world.h"

#include <math.h>

double sum = 0;
void box_newtonian_gravity(const struct BoxWorld *world_in,
                           struct BoxWorld *world_out)
{
    for (int x = 0; x < world_in->width; ++x)
    {
        for (int y = 0; y < world_in->height; ++y)
        {
            struct BoxTileData tile1 = box_tiles_data
                [world_in->tiles[BOX_INDEX(x, y, world_in->width)].type];

            for (int i = 0; i < world_in->width; ++i)
            {
                for (int j = 0; j < world_in->height; ++j)
                {
                    struct BoxTileData tile2 =
                        box_tiles_data
                            [world_in->tiles[BOX_INDEX(x, y, world_in->width)]
                                 .type];

                    double m1 = tile1.mass;
                    double m2 = tile2.mass;

                    double force =
                        (m1 * m2) / (sqrt(pow(x - i, 2) + pow(y - j, 2)));

                    int new_x = x + (force * (i - x));
                    int new_y = y + (force * (j - y));
                    sum += force;
                }
            }
        }
    }
}
