#ifndef FLECS_COMPONENTS_META_CACHE_H
#define FLECS_COMPONENTS_META_CACHE_H

#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EcsMetaCache EcsMetaCache;

typedef enum ecs_meta_cache_op_kind_t {
    EcsOpNothing,

    /* Primitives */
    EcsOpPrimitive,

    /* Enum / bitmask */
    EcsOpEnum,
    EcsOpBitmask,

    /* Struct nesting */
    EcsOpPush,
    EcsOpPop,

    /* Collections */
    EcsOpArray,
    EcsOpVector,
    EcsOpMap
} ecs_meta_cache_op_kind_t;

typedef ecs_vector_t* ecs_meta_cache_op_vector_t;

typedef struct ecs_meta_cache_op_t {
    ecs_meta_cache_op_kind_t kind;
    uint32_t offset;
    uint32_t count;
    const char *name;

    union {
        ecs_meta_cache_op_vector_t element_ops;
        EcsMetaPrimitiveKind primitive_kind;
        EcsMetaEnumConstantVector enum_constants;
        EcsMetaBitmaskConstantVector bitmask_constants;
        struct {
            struct ecs_meta_cache_op_t *key_op;
            ecs_meta_cache_op_vector_t value_ops;
        } map;
    } data;
} ecs_meta_cache_op_t;

struct EcsMetaCache {
    ecs_meta_cache_op_vector_t ops;
};

void InitCache(ecs_rows_t *rows);

#ifdef __cplusplus
}
#endif

#endif
