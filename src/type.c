#include "type.h"

ecs_entity_t ecs_meta_lookup_array(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx)
{    
    ecs_meta_parse_ctx_t param_ctx = {
        .name = ctx->name,
        .decl = params_decl
    };

    ecs_meta_params_t params;
    ecs_meta_parse_params(params_decl, &params, &param_ctx);
    if (!params.is_fixed_size) {
        ecs_meta_error(ctx, params_decl, "missing size for array");
    }

    if (!params.count) {
        ecs_meta_error(ctx, params_decl, "invalid array size");
    }

    ecs_entity_t element_type = ecs_lookup(world, params.type.type);
    if (!element_type) {
        ecs_meta_error(ctx, params_decl, "unknown element type '%s'", 
            params.type.type);
    }

    if (!e) {
        ecs_entity_t ecs_entity(EcsType) = ecs_lookup(world, "EcsType");
        ecs_assert(ecs_entity(EcsType) != 0, ECS_INTERNAL_ERROR, NULL);

        EcsType *elem_type = ecs_get_ptr(world, element_type, EcsType);
        ecs_assert(elem_type != NULL, ECS_INTERNAL_ERROR, NULL);

        e = ecs_set(world, 0, EcsType, {
            EcsArrayType, elem_type->size * params.count, elem_type->alignment
        });
    }

    ecs_entity_t ecs_entity(EcsArray) = ecs_lookup(world, "EcsArray");
    ecs_assert(ecs_entity(EcsArray) != 0, ECS_INTERNAL_ERROR, NULL);

    return ecs_set(world, e, EcsArray, { element_type, params.count });
}

ecs_entity_t ecs_meta_lookup_vector(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx)
{    
    ecs_meta_parse_ctx_t param_ctx = {
        .name = ctx->name,
        .decl = params_decl
    };

    ecs_meta_params_t params;
    ecs_meta_parse_params(params_decl, &params, &param_ctx);
    if (params.is_key_value) {
        ecs_meta_error(ctx, params_decl, 
            "unexpected key value parameters for vector");
    }

    ecs_entity_t element_type = ecs_meta_lookup(
        world, &params.type, params_decl, 1, &param_ctx);

    if (!e) {
        ecs_entity_t ecs_entity(EcsType) = ecs_lookup(world, "EcsType");
        ecs_assert(ecs_entity(EcsType) != 0, ECS_INTERNAL_ERROR, NULL);

        e = ecs_set(world, 0, EcsType, {EcsVectorType});
    }

    ecs_entity_t ecs_entity(EcsVector) = ecs_lookup(world, "EcsVector");
    ecs_assert(ecs_entity(EcsVector) != 0, ECS_INTERNAL_ERROR, NULL);

    return ecs_set(world, e, EcsVector, { element_type });
}

ecs_entity_t ecs_meta_lookup_map(
    ecs_world_t *world,
    ecs_entity_t e,    
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx)
{    
    ecs_meta_parse_ctx_t param_ctx = {
        .name = ctx->name,
        .decl = params_decl
    };

    ecs_meta_params_t params;
    ecs_meta_parse_params(params_decl, &params, &param_ctx);
    if (!params.is_key_value) {
        ecs_meta_error(ctx, params_decl, 
            "missing key type for map");
    }

    ecs_entity_t key_type = ecs_meta_lookup(
        world, &params.key_type, params_decl, 1, &param_ctx);

    ecs_entity_t element_type = ecs_meta_lookup(
        world, &params.type, params_decl, 1, &param_ctx);
    
    if (!e) {
        ecs_entity_t ecs_entity(EcsType) = ecs_lookup(world, "EcsType");
        ecs_assert(ecs_entity(EcsType) != 0, ECS_INTERNAL_ERROR, NULL);
        
        e = ecs_set(world, 0, EcsType, {EcsMapType});
    }

    ecs_entity_t ecs_entity(EcsMap) = ecs_lookup(world, "EcsMap");
    ecs_assert(ecs_entity(EcsMap) != 0, ECS_INTERNAL_ERROR, NULL);

    return ecs_set(world, e, EcsMap, { key_type, element_type });
}

ecs_entity_t ecs_meta_lookup(
    ecs_world_t *world,
    ecs_meta_type_t *token,
    const char *ptr,
    int32_t count,
    ecs_meta_parse_ctx_t *ctx)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(token != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ctx != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *typename = token->type;
    ecs_entity_t type = 0;

    /* Parse vector type */
    if (!strcmp(typename, "ecs_array")) {
        type = ecs_meta_lookup_array(world, 0, token->params, ctx);

    } else if (!strcmp(typename, "ecs_vector")) {
        type = ecs_meta_lookup_vector(world, 0, token->params, ctx);

    } else if (!strcmp(typename, "ecs_map")) {
        type = ecs_meta_lookup_map(world, 0, token->params, ctx);

    } else {
        if (token->is_ptr && !strcmp(typename, "char")) {
            typename = "ecs_string_t";
        }

        type = ecs_lookup(world, typename);
        if (!type) {
            ecs_meta_error(ctx, ptr, "unknown type '%s'", typename);
        }
    }

    return type;
}
