#ifndef FLECS_COMPONENTS_META_API_H
#define FLECS_COMPONENTS_META_API_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Convenience macro's */

#define ECS_DEFINE(world, component)\
    ecs_add(world, ecs_entity(component), EcsMetaDefined)

#define ECS_PRIMITIVE(world, component, primitive_kind)\
    ecs_set(world, ecs_entity(component), EcsMetaPrimitive, {primitive_kind});\
    ECS_DEFINE(world, component)

#define ECS_ENUM_CONSTANT(world, component, constant)\
    ecs_enum_push_constant(world, ecs_entity(component), #constant, constant)

#define ECS_BITMASK_CONSTANT(world, component, constant)\
    ecs_bitmask_push_constant(world, ecs_entity(component), #constant, constant)

#define ECS_STRUCT_MEMBER(world, component, member, type)\
    ecs_struct_push_member(world, ecs_entity(component), #member, ecs_entity(type), offsetof(component, member))

#define ECS_ARRAY(world, component, element_type, size)\
    ecs_set(world, ecs_entity(component), EcsMetaArray, {ecs_entity(element_type), size});\
    ECS_DEFINE(world, component)

#define ECS_VECTOR(world, component, element_type, max_size)\
    ecs_set(world, ecs_entity(component), EcsMetaVector, {ecs_entity(element_type), max_size});\
    ECS_DEFINE(world, component)

#define ECS_MAP(world, component, key_type, value_type, max_size)\
    ecs_set(world, ecs_entity(component), EcsMetaVector, {ecs_entity(element_type), max_size});\
    ECS_DEFINE(world, component)


/* Meta API */

FLECS_COMPONENTS_META_EXPORT
void ecs_enum_push_constant(
    ecs_world_t *world,
    ecs_entity_t component,
    const char *name,
    int32_t value);

FLECS_COMPONENTS_META_EXPORT
void ecs_bitmask_push_constant(
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
