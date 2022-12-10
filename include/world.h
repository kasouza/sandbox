#ifndef KASOUZA_SANDBOX_INCLUDE_WORLD_H
#define KASOUZA_SANDBOX_INCLUDE_WORLD_H

#define BOX_MAX_SYSTEMS 32

// For column major things
#define BOX_TILE_INDEX(x, y, world) ((x) + (y) * (world->width))
#define BOX_TILE_AT(x, y, world) (world->tiles[BOX_TILE_INDEX((x), (y), world)])
#define BOX_TILE_DATA(tile) (box_tile_data_map[tile.type])

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

enum BoxTileType
{
    BOX_TILE_EMPTY,
    BOX_TILE_BARRIER,
    BOX_TILE_AIR,
    BOX_TILE_DIRT,
    BOX_TILE_IRON,
    BOX_TILE_CGOL_CELL,
	BOX_TILE_WATER,

    // For Initializing arrays and stuff
    BOX_TILE_LAST,
};

enum BoxSimulationType
{
    BOX_SOLID,
    BOX_SAND,
    BOX_LIQUID,
    BOX_GAS,
};

struct BoxTileData
{
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } color;

    double mass;
    enum BoxSimulationType simulation_type;
};

extern const struct BoxTileData box_tile_data_map[BOX_TILE_LAST];

struct BoxTile
{
    enum BoxTileType type;
};

extern const struct BoxTile box_empty_tile;

struct BoxWorld
{
    int width;
    int height;
    struct BoxTile *tiles;
};

struct BoxWorld *box_create_world(int width, int height);
void box_free_world(struct BoxWorld *world);

#endif
