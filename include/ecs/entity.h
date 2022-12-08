#ifndef KASOUZA_MINI_SURVIVAL_ECS_ENTITY_H
#define KASOUZA_MINI_SURVIVAL_ECS_ENTITY_H

#define MAX_ENTITIES 32

typedef unsigned long entity_id_t;
typedef unsigned long entity_idx_t;

entity_id_t create_entity();
void destroy_entity(entity_id_t entity);

void test_id_queue();

#endif
