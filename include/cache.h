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
    uint16_t size;
    uint16_t count;
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

/* Serialize API */

void* ecs_meta_get_ptr(
    void *base, 
    ecs_meta_cache_op_t *op);

bool ecs_meta_get_bool(
    void *base,
    ecs_meta_cache_op_t *op);

char ecs_meta_get_char(
    void *base,
    ecs_meta_cache_op_t *op);

int64_t ecs_meta_get_int(
    void *base,
    ecs_meta_cache_op_t *op);

uint64_t ecs_meta_get_uint(
    void *base,
    ecs_meta_cache_op_t *op);

int32_t ecs_meta_get_enum(
    void *base,
    ecs_meta_cache_op_t *op);

uint64_t ecs_meta_get_bitmask(
    void *base,
    ecs_meta_cache_op_t *op);

double ecs_meta_get_float(
    void *base,
    ecs_meta_cache_op_t *op);

uintptr_t ecs_meta_get_word(
    void *base,
    ecs_meta_cache_op_t *op);

char* ecs_meta_get_string(
    void *base,
    ecs_meta_cache_op_t *op);

ecs_entity_t ecs_meta_get_entity(
    void *base,
    ecs_meta_cache_op_t *op);

const char* ecs_meta_enum_to_string(
    int32_t value,
    ecs_meta_cache_op_t *op);

void ecs_meta_bitmask_to_string(
    uint64_t value,
    ecs_meta_cache_op_t *op,
    ut_strbuf *buf);

ecs_vector_t* ecs_meta_get_vector(
    void *base,
    ecs_meta_cache_op_t *op);

#ifdef __cplusplus
}
#endif

#endif
