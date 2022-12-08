#ifndef KASOUZA_MINI_SURVIVAL_ECS_COMPONENT_H
#define KASOUZA_MINI_SURVIVAL_ECS_COMPONENT_H

#include "ecs/entity.h"

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stddef.h>

#define INIT_COMPONENT(component_t)\
mappings.component_t##_entity_to_idx[i] = MAX_ENTITIES;\
mappings.component_t##_idx_to_entity[i] = MAX_ENTITIES;

#define REGISTER_COMPONENT(component_t)\
size_t component_t##_size;\
struct component_t component_t[MAX_ENTITIES];

#define REGISTER_COMPONENT_MAPPINGS(component_t)\
entity_idx_t component_t##_entity_to_idx[MAX_ENTITIES];\
entity_id_t component_t##_idx_to_entity[MAX_ENTITIES];

#define DECLARE_GET_COMPONENT(component_t, component_ptr_t) component_ptr_t get_##component_t##_component(entity_id_t entity)
#define DECLARE_REMOVE_COMPONENT(component_type) void remove_##component_type##_component(entity_id_t entity)

#define BOILERPLATE_ADD_COMPONENT(component_t)\
entity_idx_t idx = components.component_t##_size++;\
mappings.component_t##_entity_to_idx[entity] = idx;\
mappings.component_t##_idx_to_entity[idx] = entity;\
components.component_t[idx].entity = entity;

#define DEFINE_GET_COMPONENT(component_t, component_ptr_t)\
component_ptr_t get_##component_t##_component(entity_id_t entity)\
{\
	entity_idx_t idx = mappings.component_t##_entity_to_idx[entity];\
	return &components.component_t[idx];\
}

#define DEFINE_REMOVE_COMPONENT(component_t)\
void remove_##component_t##_component(entity_id_t entity)\
{\
	entity_idx_t idx = mappings.component_t##_entity_to_idx[entity];\
	entity_idx_t new_component_idx = --components.component_t##_size;\
	if (idx == new_component_idx)\
	{\
		--new_component_idx;\
	}\
\
	components.component_t[idx] = components.component_t[new_component_idx];\
	entity_id_t new_entity = mappings.component_t##_idx_to_entity[new_component_idx];\
\
	mappings.component_t##_entity_to_idx[new_entity] = idx;\
	mappings.component_t##_idx_to_entity[idx] = new_entity;\
\
	mappings.component_t##_entity_to_idx[idx] = MAX_ENTITIES;\
	mappings.component_t##_idx_to_entity[new_component_idx] = MAX_ENTITIES;\
}

extern struct Components components;
extern struct Mappings mappings;

struct position
{
	entity_id_t entity;
	int x;
	int y;
};

typedef struct position* position_ptr_t;
void add_position_component(entity_id_t id, int x, int y);
DECLARE_GET_COMPONENT(position, position_ptr_t);
DECLARE_REMOVE_COMPONENT(position);

struct color
{
	entity_id_t entity;
	int color;
};

typedef struct color* color_ptr_t;
void add_color_component(entity_id_t id, int clr);
DECLARE_GET_COMPONENT(color, color_ptr_t);
DECLARE_REMOVE_COMPONENT(color);

struct sprite
{
	entity_id_t entity;
	SDL_Texture* texture;
	SDL_Rect src_rect;
	SDL_Rect dest_rect;
	int layer;
};

typedef struct sprite* sprite_ptr_t;
void add_sprite_component(entity_id_t id, SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, int layer);
DECLARE_GET_COMPONENT(sprite, sprite_ptr_t);
DECLARE_REMOVE_COMPONENT(sprite);

struct animation
{
	entity_id_t entity;
	int x_idx;
	int y_idx;
	int max_x_idx;
	int max_y_idx;
	int delay;
	int max_delay;
};

typedef struct animation* animation_ptr_t;
void add_animation_component(entity_id_t id, int delay);
DECLARE_GET_COMPONENT(animation, animation_ptr_t);
DECLARE_REMOVE_COMPONENT(animation);

struct Mappings
{
	REGISTER_COMPONENT_MAPPINGS(position)
	REGISTER_COMPONENT_MAPPINGS(color)
	REGISTER_COMPONENT_MAPPINGS(sprite)
	REGISTER_COMPONENT_MAPPINGS(animation)
};

struct Components
{
	REGISTER_COMPONENT(position)
	REGISTER_COMPONENT(color)
	REGISTER_COMPONENT(sprite)
	REGISTER_COMPONENT(animation)
};

#endif
