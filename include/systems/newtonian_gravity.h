#ifndef KASOUZA_SANDBOX_INCLUDE_SYSTEMS_NEWTONIAN_GRAVITY_H
#define KASOUZA_SANDBOX_INCLUDE_SYSTEMS_NEWTONIAN_GRAVITY_H

struct BoxWorld;
void box_newtonian_gravity(const struct BoxWorld *world_in,
                           struct BoxWorld *world_out);

#endif
