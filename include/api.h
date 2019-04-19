#ifndef FLECS_COMPONENTS_META_API_H
#define FLECS_COMPONENTS_META_API_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ECS_ENUM_CONSTANT(world, component, constant)\
    ecs_enum_push_constant(world, ecs_to_entity(component), #constant, constant)

#define ECS_STRUCT_MEMBER(world, component, member, type)\
    ecs_struct_push_member(world, ecs_to_entity(component), #member, ecs_to_entity(type), offsetof(component, member))

FLECS_COMPONENTS_META_EXPORT
void ecs_enum_push_constant(
    ecs_world_t *world,
    ecs_entity_t component,
    const char *name,
    int32_t value);

FLECS_COMPONENTS_META_EXPORT
void ecs_struct_push_member(
    ecs_world_t *world,
    ecs_entity_t component,
    const char *name,
    ecs_entity_t type,
    uint32_t offset);

#ifdef __cplusplus
}
#endif

#endif
