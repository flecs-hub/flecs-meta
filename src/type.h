#ifndef FLECS_META_TYPE_H
#define FLECS_META_TYPE_H

#include "parser.h"

ecs_entity_t ecs_meta_lookup_array(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx);

ecs_entity_t ecs_meta_lookup_vector(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx);

ecs_entity_t ecs_meta_lookup_map(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx);

ecs_entity_t ecs_meta_lookup(
    ecs_world_t *world,
    ecs_meta_type_t *token,
    const char *ptr,
    int64_t count,
    ecs_meta_parse_ctx_t *ctx);

#endif
