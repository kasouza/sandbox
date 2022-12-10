#ifndef KASOUZA_SANDBOX_INCLUDE_SYSTEMS_CONWAYS_GAME_OF_LIFE_H
#define KASOUZA_SANDBOX_INCLUDE_SYSTEMS_CONWAYS_GAME_OF_LIFE_H

struct BoxWorld;
void conways_game_of_life(const struct BoxWorld *world_in,
                          struct BoxWorld *world_out);

#endif
