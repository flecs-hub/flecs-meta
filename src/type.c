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

    ecs_entity_t element_type = ecs_lookup_symbol(world, params.type.type);
    if (!element_type) {
        ecs_meta_error(ctx, params_decl, "unknown element type '%s'", 
            params.type.type);
    }

    if (!e) {
        ecs_entity_t ecs_entity(EcsMetaType) = ecs_lookup_fullpath(world, "flecs.meta.MetaType");
        ecs_assert(ecs_entity(EcsMetaType) != 0, ECS_INTERNAL_ERROR, NULL);

        const EcsMetaType *elem_type = ecs_get(world, element_type, EcsMetaType);
        ecs_assert(elem_type != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_assert(elem_type->size * params.count <= INT32_MAX, 
            ECS_INVALID_PARAMETER, NULL);

        e = ecs_set(world, 0, EcsMetaType, {
            EcsArrayType, (int32_t)(elem_type->size * params.count), 
            elem_type->alignment, NULL
        });
    }

    ecs_entity_t ecs_entity(EcsArray) = ecs_lookup_fullpath(world, "flecs.meta.Array");
    ecs_assert(ecs_entity(EcsArray) != 0, ECS_INTERNAL_ERROR, NULL);

    ecs_assert(params.count <= INT32_MAX, ECS_INVALID_PARAMETER, NULL);

    return ecs_set(world, e, EcsArray, { element_type, (int32_t)params.count });
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
        ecs_entity_t ecs_entity(EcsMetaType) = ecs_lookup_fullpath(world, "flecs.meta.MetaType");
        ecs_assert(ecs_entity(EcsMetaType) != 0, ECS_INTERNAL_ERROR, NULL);

        e = ecs_set(world, 0, EcsMetaType, {EcsVectorType, 0, 0, NULL});
    }

    ecs_entity_t ecs_entity(EcsVector) = ecs_lookup_fullpath(world, "flecs.meta.Vector");
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
        ecs_entity_t ecs_entity(EcsMetaType) = ecs_lookup_fullpath(world, "flecs.meta.MetaType");
        ecs_assert(ecs_entity(EcsMetaType) != 0, ECS_INTERNAL_ERROR, NULL);
        
        e = ecs_set(world, 0, EcsMetaType, {EcsMapType, 0, 0, NULL});
    }

    ecs_entity_t ecs_entity(EcsMap) = ecs_lookup_fullpath(world, "flecs.meta.Map");
    ecs_assert(ecs_entity(EcsMap) != 0, ECS_INTERNAL_ERROR, NULL);

    return ecs_set(world, e, EcsMap, { key_type, element_type });
}

ecs_entity_t ecs_meta_lookup_bitmask(
    ecs_world_t *world,
    ecs_entity_t e,
    const char *params_decl,
    ecs_meta_parse_ctx_t *ctx)
{
    (void)e;

    ecs_meta_parse_ctx_t param_ctx = {
        .name = ctx->name,
        .decl = params_decl
    };

    ecs_meta_params_t params;
    ecs_meta_parse_params(params_decl, &params, &param_ctx);
    if (params.is_key_value) {
        ecs_meta_error(ctx, params_decl, 
            "unexpected key value parameters for bitmask");
    }

    if (params.is_fixed_size) {
        ecs_meta_error(ctx, params_decl, 
            "unexpected size for bitmask");
    }

    ecs_entity_t bitmask_type = ecs_meta_lookup(
        world, &params.type, params_decl, 1, &param_ctx);
    ecs_assert(bitmask_type != 0, ECS_INVALID_PARAMETER, NULL);

    /* Make sure this is a bitmask type */
    ecs_entity_t ecs_entity(EcsMetaType) = ecs_lookup_fullpath(world, "flecs.meta.MetaType");
    ecs_assert(ecs_entity(EcsMetaType) != 0, ECS_INTERNAL_ERROR, NULL);

    const EcsMetaType *type_ptr = ecs_get(world, bitmask_type, EcsMetaType);
    ecs_assert(type_ptr != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(type_ptr->kind == EcsBitmaskType, ECS_INVALID_PARAMETER, NULL);

    return bitmask_type;
}

ecs_entity_t ecs_meta_lookup(
    ecs_world_t *world,
    ecs_meta_type_t *token,
    const char *ptr,
    int64_t count,
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

    } else if (!strcmp(typename, "ecs_vector") || !strcmp(typename, "flecs::vector")) {
        type = ecs_meta_lookup_vector(world, 0, token->params, ctx);

    } else if (!strcmp(typename, "ecs_map") | !strcmp(typename, "flecs::map")) {
        type = ecs_meta_lookup_map(world, 0, token->params, ctx);

    } else if (!strcmp(typename, "flecs::bitmask")) {
        type = ecs_meta_lookup_bitmask(world, 0, token->params, ctx);

    } else if (!strcmp(typename, "flecs::byte")) {
        type = ecs_lookup(world, "ecs_byte_t");

    } else {
        if (token->is_ptr && !strcmp(typename, "char")) {
            typename = "ecs_string_t";
        } else
        if (token->is_ptr) {
            typename = "uintptr_t";
        } else        
        if (!strcmp(typename, "char*") || !strcmp(typename, "flecs::string")) {
            typename = "ecs_string_t";
        }

        type = ecs_lookup_symbol(world, typename);
        if (!type) {
            ecs_meta_error(ctx, ptr, "unknown type '%s'", typename);
            return 0;
        }
    }

    if (count != 1) {
        /* If count is not 1, insert array type. First lookup EcsMetaType of the
         * element type to get the size and alignment. Then create a new
         * entity for the array type, and assign it to the member type. */
        ecs_entity_t ecs_entity(EcsMetaType) = ecs_lookup_fullpath(world, "flecs.meta.MetaType");
        ecs_entity_t ecs_entity(EcsArray) = ecs_lookup_fullpath(world, "flecs.meta.Array");
        const EcsMetaType *type_ptr = ecs_get(world, type, EcsMetaType);

        ecs_assert(count <= INT32_MAX, ECS_INVALID_PARAMETER, NULL);

        type = ecs_set(world, ecs_set(world, 0, 
            EcsMetaType, {EcsArrayType, type_ptr->size, type_ptr->alignment, NULL}),
            EcsArray, {type, (int32_t)count});
    }

    return type;
}
