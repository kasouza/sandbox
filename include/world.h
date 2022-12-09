#ifndef KASOUZA_SANDBOX_INCLUDE_WORLD_H
#define KASOUZA_SANDBOX_INCLUDE_WORLD_H

#define WORLD_SIZE 256
#define BOX_MAX_SYSTEMS 4

// #define BOX_SYSTEM_NO_OUTPUT 			0x00000001
// #define BOX_SYSTEM_EMPTY_OUTPUT_BUFFER 	0x00000010

enum BoxSystemFlags
{
    // The output of this system will be ignored and the input will used as the
    // input for the next system.
    BOX_SYSTEM_PASSTHROUGH = 0x00000001,

    // The output world tiles will all be initialized to 0 if AND
    // ONLY IF flags is set.
    BOX_SYSTEM_EMPTY_OUTPUT_BUFFER = 0x00000010

};

struct BoxWorld;
typedef void (*box_system_t)(const struct BoxWorld *world_in,
                             struct BoxWorld *world_out);

enum BoxTileType
{
    NONE,
    AIR,
    DIRT
};

struct BoxTile
{
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } color;
};

struct BoxWorld
{
    int width;
    int height;
    struct BoxTile *tiles;

    struct
    {
        box_system_t run;
        int flags;
    } systems[BOX_MAX_SYSTEMS];
    int system_count;
};

struct BoxWorld *box_create_world(int width, int height);
void box_free_world(struct BoxWorld *world);

int box_add_system(struct BoxWorld *world, box_system_t system,
                   int system_flags);

void box_set_system(struct BoxWorld *world, int system_id, box_system_t system,
                    int system_flags);

void box_remove_system(struct BoxWorld *world, int system_id);
void box_tick_world(struct BoxWorld *world);

#endif
