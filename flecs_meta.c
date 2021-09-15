#ifndef FLECS_META_IMPL
#include "flecs_meta.h"
#endif
#ifndef FLECS_META_PARSER_H
#define FLECS_META_PARSER_H


#define ECS_META_IDENTIFIER_LENGTH (256)

#define ecs_meta_error(ctx, ptr, ...)\
    ecs_parser_error((ctx)->name, (ctx)->decl, ptr - (ctx)->decl, __VA_ARGS__)

typedef char ecs_meta_token_t[ECS_META_IDENTIFIER_LENGTH];

typedef struct ecs_meta_parse_ctx_t {
    const char *name;
    const char *decl;
} ecs_meta_parse_ctx_t;

typedef struct ecs_meta_type_t {
    ecs_meta_token_t type;
    ecs_meta_token_t params;
    bool is_const;
    bool is_ptr;
} ecs_meta_type_t;

typedef struct ecs_meta_member_t {
    ecs_meta_type_t type;
    ecs_meta_token_t name;
    int64_t count;
    bool is_partial;
} ecs_meta_member_t;

typedef struct ecs_meta_constant_t {
    ecs_meta_token_t name;
    int64_t value;
    bool is_value_set;
} ecs_meta_constant_t;

typedef struct ecs_meta_params_t {
    ecs_meta_type_t key_type;
    ecs_meta_type_t type;
    int64_t count;
    bool is_key_value;
    bool is_fixed_size;
} ecs_meta_params_t;

const char* ecs_meta_parse_constant(
    const char *ptr,
    ecs_meta_constant_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

const char* ecs_meta_parse_member(
    const char *ptr,
    ecs_meta_member_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

void ecs_meta_parse_params(
    const char *ptr,
    ecs_meta_params_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

#endif
#ifndef FLECS_META_SERIALIZER_H
#define FLECS_META_SERIALIZER_H


void EcsAddStruct(
    ecs_iter_t *it);

void EcsSetPrimitive(
    ecs_iter_t *it);

void EcsSetEnum(
    ecs_iter_t *it);

void EcsSetBitmask(
    ecs_iter_t *it);

void EcsSetStruct(
    ecs_iter_t *it);

void EcsSetArray(
    ecs_iter_t *it);

void EcsSetVector(
    ecs_iter_t *it);

void EcsSetMap(
    ecs_iter_t *it);

#endif

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    int32_t offset);

static
ecs_size_t ecs_get_primitive_size(
    ecs_primitive_kind_t kind)
{
    switch(kind) {
    case EcsBool: return sizeof(bool);
    case EcsChar: return sizeof(char);
    case EcsByte: return sizeof(char);
    case EcsU8: return sizeof(uint8_t);
    case EcsU16: return sizeof(uint16_t);
    case EcsU32: return sizeof(uint32_t);
    case EcsU64: return sizeof(uint64_t);
    case EcsI8: return sizeof(int8_t);
    case EcsI16: return sizeof(int16_t);
    case EcsI32: return sizeof(int32_t);
    case EcsI64: return sizeof(int64_t);
    case EcsF32: return sizeof(float);
    case EcsF64: return sizeof(double);
    case EcsIPtr: return sizeof(intptr_t);
    case EcsUPtr: return sizeof(uintptr_t);
    case EcsString: return sizeof(char*);
    case EcsEntity: return sizeof(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
int16_t ecs_get_primitive_alignment(
    ecs_primitive_kind_t kind)
{
    switch(kind) {
    case EcsBool: return ECS_ALIGNOF(bool);
    case EcsChar: return ECS_ALIGNOF(char);
    case EcsByte: return ECS_ALIGNOF(char);
    case EcsU8: return ECS_ALIGNOF(uint8_t);
    case EcsU16: return ECS_ALIGNOF(uint16_t);
    case EcsU32: return ECS_ALIGNOF(uint32_t);
    case EcsU64: return ECS_ALIGNOF(uint64_t);
    case EcsI8: return ECS_ALIGNOF(int8_t);
    case EcsI16: return ECS_ALIGNOF(int16_t);
    case EcsI32: return ECS_ALIGNOF(int32_t);
    case EcsI64: return ECS_ALIGNOF(int64_t);
    case EcsF32: return ECS_ALIGNOF(float);
    case EcsF64: return ECS_ALIGNOF(double);
    case EcsIPtr: return ECS_ALIGNOF(intptr_t);
    case EcsUPtr: return ECS_ALIGNOF(uintptr_t);
    case EcsString: return ECS_ALIGNOF(char*);
    case EcsEntity: return ECS_ALIGNOF(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
ecs_vector_t* serialize_primitive(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsPrimitive *type,
    ecs_vector_t *ops)
{
    (void)world;
    (void)entity;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = ecs_get_primitive_size(type->kind),
            .alignment = ecs_get_primitive_alignment(type->kind)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    *op = (ecs_type_op_t) {
        .kind = EcsOpPrimitive,
        .size = ecs_get_primitive_size(type->kind),
        .alignment = ecs_get_primitive_alignment(type->kind),
        .count = 1,
        .is.primitive = type->kind
    };

    return ops;
}

static
ecs_vector_t* serialize_enum(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsEnum *type,
    ecs_vector_t *ops)
{
    (void)type;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, entity, EcsEnum);

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_bitmask(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsBitmask *type,
    ecs_vector_t *ops)
{
    (void)type;

    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, entity, EcsBitmask);

    *op = (ecs_type_op_t) {
        .kind = EcsOpBitmask,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_struct(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsStruct *type,
    ecs_vector_t *ops,
    int32_t offset)
{
    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    int32_t push_op = ecs_vector_count(ops);

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPush
    };

    ecs_size_t size = 0;
    int16_t alignment = 0;

    EcsMember *members = ecs_vector_first(type->members, EcsMember);
    int32_t i, count = ecs_vector_count(type->members);

    for (i = 0; i < count; i ++) {
        /* Add type operations of member to struct ops */
        int32_t prev_count = ecs_vector_count(ops);
        ops = serialize_type(world, members[i].type, ops, offset + size);

#ifndef NDEBUG
        int32_t op_count = ecs_vector_count(ops);

        /* At least one op should be added */
        ecs_assert(prev_count != op_count, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(ops != NULL, ECS_INTERNAL_ERROR, NULL);
#endif

        op = ecs_vector_get(ops, ecs_type_op_t, prev_count);
        op->name = members[i].name;

        const EcsMetaType *meta_type = ecs_get(world, members[i].type, EcsMetaType);
        ecs_size_t member_size = meta_type->size * (meta_type->kind == EcsStructType ? 1 : op->count);
        int16_t member_alignment = meta_type->alignment;

        ecs_assert(member_size != 0, ECS_INTERNAL_ERROR, op->name);
        ecs_assert(member_alignment != 0, ECS_INTERNAL_ERROR, op->name);

        size = ECS_ALIGN(size, member_alignment);
        op->offset = offset + size;

        size += member_size;

        if (member_alignment > alignment) {
            alignment = member_alignment;
        }
    }

    /* Align struct size to struct alignment */
    size = ECS_ALIGN(size, alignment);

    /* Size and alignment are ordinarily determined by ECS_STRUCT and should be
     * the same as the values computed here. However, there are two exceptions.
     * The first exception is when an application defines a type by populating
     * the EcsStruct component directly and does not provide size and alignment
     * values for EcsMetaType. The second scenario is when the type definition
     * contains an ECS_PRIVATE, in which case the type may contain
     * members that are not described.
     *
     * In the first case the computed values should be set in EcsMetaType. In the
     * second case the values from EcsMetaType should be assigned to the type
     * operation. */
    bool is_added;
    EcsMetaType *base_type = ecs_get_mut(world, entity, EcsMetaType, &is_added);
    ecs_assert(base_type != NULL, ECS_INTERNAL_ERROR, NULL);
    if (!is_added) {
        if (!type->is_partial) {
            /* EcsMetaType existed already, and this is not a partial type. This
             * means that computed size and alignment should match exactly. */
            if (base_type->size) {
                ecs_assert(base_type->size == size, ECS_INTERNAL_ERROR, NULL);
            }

            if (base_type->alignment) {
                ecs_assert(
                    base_type->alignment == alignment, ECS_INTERNAL_ERROR, NULL);
            }
        } else {
            /* EcsMetaType exists, and this is a partial type. In this case the
             * computed values only apply to the members described in EcsStruct
             * but not to the type as a whole. Use the values from EcsMetaType. Note
             * that it is not allowed to have a partial type for which no size
             * and alignment are specified in EcsMetaType. */
            ecs_assert(base_type->size != 0, ECS_INVALID_PARAMETER, NULL);
            ecs_assert(base_type->alignment != 0, ECS_INVALID_PARAMETER, NULL);

            size = base_type->size;
            alignment = base_type->alignment;
        }
    } else {
        /* If EcsMetaType was not set yet, initialize descriptor, alias to NULL
         * since it won't be used here */
        base_type->descriptor = NULL;
        base_type->alias = NULL;
    }

    base_type->kind = EcsStructType;
    base_type->size = size;
    base_type->alignment = alignment;

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPop,
    };

    if (op_header) {
        op_header = ecs_vector_first(ops, ecs_type_op_t);
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = size,
            .alignment = alignment
        };
    }

    ecs_type_op_t *op_push = ecs_vector_get(ops, ecs_type_op_t, push_op);
    ecs_assert(op_push->kind == EcsOpPush, ECS_INTERNAL_ERROR, NULL);
    op_push->size = size;
    op_push->alignment = alignment;
    op_push->count = count;

    return ops;
}

static
ecs_vector_t* serialize_array(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsArray *type,
    ecs_vector_t *ops)
{
    (void)entity;

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    const EcsMetaType *element_type = ecs_get(world, type->element_type, EcsMetaType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, type->element_type, EcsMetaTypeSerializer);

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpArray,
        .count = type->count,
        .size = element_type->size,
        .alignment = element_type->alignment,
        .is.collection = ref
    };

    if (op_header) {
        op_header = ecs_vector_first(ops, ecs_type_op_t);
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = op->size,
            .alignment = op->alignment
        };
    }

    EcsMetaType *meta = ecs_get_mut(world, entity, EcsMetaType, NULL);

    if(!meta->size || !meta->alignment) {
        meta->size = type->count * element_type->size;
        meta->alignment = element_type->alignment;
    }

    return ops;
}

static
ecs_vector_t* serialize_vector(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsVector *type,
    ecs_vector_t *ops)
{
    (void)entity;

    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_vector_t*),
            .alignment = ECS_ALIGNOF(ecs_vector_t*)
        };
    }

    const EcsMetaType *element_type = ecs_get(world, type->element_type, EcsMetaType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_ref_t ref = {0};
    ecs_get_ref(world, &ref, type->element_type, EcsMetaTypeSerializer);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpVector,
        .count = 1,
        .size = element_type->size,
        .alignment = element_type->alignment,
        .is.collection = ref
    };

    return ops;
}

static
ecs_vector_t* serialize_map(
    ecs_world_t *world,
    ecs_entity_t entity,
    const EcsMap *type,
    ecs_vector_t *ops)
{
    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_map_t*),
            .alignment = ECS_ALIGNOF(ecs_map_t*)
        };
    }

    const EcsMetaTypeSerializer *key_cache = ecs_get(world, type->key_type, EcsMetaTypeSerializer);
    ecs_assert(key_cache != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(key_cache->ops != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ecs_vector_count(key_cache->ops) != 0, ECS_INTERNAL_ERROR, NULL);

    /* Make sure first op is the header */
    ecs_type_op_t *key_op = ecs_vector_first(key_cache->ops, ecs_type_op_t);
    ecs_assert(key_op->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);

    if (ecs_vector_count(key_cache->ops) != 2) {
        const EcsMetaType *ptr = ecs_get(world, entity, EcsMetaType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_name(world, entity),
            .decl = ptr->descriptor
        };

        ecs_meta_error( &ctx, ctx.decl,
            "invalid key type '%s' for map", ecs_get_name(world, type->key_type));
    }

    key_op = ecs_vector_get(key_cache->ops, ecs_type_op_t, 1);
    ecs_assert(key_op != NULL, ECS_INTERNAL_ERROR, NULL);

    if (key_op->count != 1) {
        const EcsMetaType *ptr = ecs_get(world, entity, EcsMetaType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_name(world, entity),
            .decl = ptr->descriptor
        };
        ecs_meta_error( &ctx, ctx.decl, "array type invalid for key type");
    }

    ecs_ref_t key_ref = {0};
    ecs_get_ref(world, &key_ref, type->key_type, EcsMetaTypeSerializer);

    ecs_ref_t element_ref = {0};
    ecs_get_ref(world, &element_ref, type->element_type, EcsMetaTypeSerializer);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpMap,
        .count = 1,
        .size = sizeof(ecs_map_t*),
        .alignment = ECS_ALIGNOF(ecs_map_t*),
        .is.map = {
            .key = key_ref,
            .element = element_ref
        }
    };

    return ops;
}

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    int32_t offset)
{
    const EcsMetaType *type = ecs_get(world, entity, EcsMetaType);
    ecs_assert(type != NULL, ECS_INVALID_PARAMETER, NULL);

    switch(type->kind) {
    case EcsPrimitiveType: {
        const EcsPrimitive *t = ecs_get(world, entity, EcsPrimitive);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_primitive(world, entity, t, ops);
    }

    case EcsEnumType: {
        const EcsEnum *t = ecs_get(world, entity, EcsEnum);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_enum(world, entity, t, ops);
    }

    case EcsBitmaskType: {
        const EcsBitmask *t = ecs_get(world, entity, EcsBitmask);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_bitmask(world, entity, t, ops);
    }

    case EcsStructType: {
        const EcsStruct *t = ecs_get(world, entity, EcsStruct);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_struct(world, entity, t, ops, offset);
    }

    case EcsArrayType: {
        const EcsArray *t = ecs_get(world, entity, EcsArray);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_array(world, entity, t, ops);
    }

    case EcsVectorType: {
        const EcsVector *t = ecs_get(world, entity, EcsVector);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_vector(world, entity, t, ops);
    }

    case EcsMapType: {
        const EcsMap *t = ecs_get(world, entity, EcsMap);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_map(world, entity, t, ops);
    }

    default:
        break;
    }

    return NULL;
}

void EcsSetPrimitive(ecs_iter_t *it) {
    EcsPrimitive *type = ecs_term(it, EcsPrimitive, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

        /* Size and alignment for primitive types can only be set after we know
         * what kind of primitive type it is. Set values in case they haven't
         * been set already */
        bool is_added;
        EcsMetaType *base_type = ecs_get_mut(world, e, EcsMetaType, &is_added);
        ecs_assert(base_type != NULL, ECS_INTERNAL_ERROR, NULL);

        base_type->size = ecs_get_primitive_size(type[i].kind);
        base_type->alignment = ecs_get_primitive_alignment(type[i].kind);

        ecs_set(world, e, EcsMetaTypeSerializer, {
            serialize_primitive(
                world, e, &type[i], NULL)
        });
    }
}

void EcsSetEnum(ecs_iter_t *it) {
    EcsEnum *type = ecs_term(it, EcsEnum, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_enum(world, e, &type[i], NULL)
        });
    }
}

void EcsSetBitmask(ecs_iter_t *it) {
    EcsBitmask *type = ecs_term(it, EcsBitmask, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_bitmask(world, e, &type[i], NULL)
        });
    }
}

void EcsSetStruct(ecs_iter_t *it) {
    EcsStruct *type = ecs_term(it, EcsStruct, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_struct(world, e, &type[i], NULL, 0)
        });
    }
}

void EcsSetArray(ecs_iter_t *it) {
    EcsArray *type = ecs_term(it, EcsArray, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_array(world, e, &type[i], NULL)
        });
    }
}

void EcsSetVector(ecs_iter_t *it) {
    EcsVector *type = ecs_term(it, EcsVector, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_vector(world, e, &type[i], NULL)
        });
    }
}

void EcsSetMap(ecs_iter_t *it) {
    EcsMap *type = ecs_term(it, EcsMap, 1);

    ecs_world_t *world = it->world;

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        ecs_set(it->world, e, EcsMetaTypeSerializer, {
            serialize_map(world, e, &type[i], NULL)
        });
    }
}

char* ecs_chresc(
    char *out, 
    char in, 
    char delimiter) 
{
    char *bptr = out;
    switch(in) {
    case '\a':
        *bptr++ = '\\';
        *bptr = 'a';
        break;
    case '\b':
        *bptr++ = '\\';
        *bptr = 'b';
        break;
    case '\f':
        *bptr++ = '\\';
        *bptr = 'f';
        break;
    case '\n':
        *bptr++ = '\\';
        *bptr = 'n';
        break;
    case '\r':
        *bptr++ = '\\';
        *bptr = 'r';
        break;
    case '\t':
        *bptr++ = '\\';
        *bptr = 't';
        break;
    case '\v':
        *bptr++ = '\\';
        *bptr = 'v';
        break;
    case '\\':
        *bptr++ = '\\';
        *bptr = '\\';
        break;
    default:
        if (in == delimiter) {
            *bptr++ = '\\';
            *bptr = delimiter;
        } else {
            *bptr = in;
        }
        break;
    }

    *(++bptr) = '\0';

    return bptr;
}

const char* ecs_chrparse(
    const char *in, 
    char *out) 
{
    const char *result = in + 1;
    char ch;

    if (in[0] == '\\') {
        result ++;

        switch(in[1]) {
        case 'a':
            ch = '\a';
            break;
        case 'b':
            ch = '\b';
            break;
        case 'f':
            ch = '\f';
            break;
        case 'n':
            ch = '\n';
            break;
        case 'r':
            ch = '\r';
            break;
        case 't':
            ch = '\t';
            break;
        case 'v':
            ch = '\v';
            break;
        case '\\':
            ch = '\\';
            break;
        case '"':
            ch = '"';
            break;
        case '0':
            ch = '\0';
            break;
        case ' ':
            ch = ' ';
            break;
        case '$':
            ch = '$';
            break;
        default:
            goto error;
        }
    } else {
        ch = in[0];
    }

    if (out) {
        *out = ch;
    }

    return result;
error:
    return NULL;
}

ecs_size_t ecs_stresc(
    char *out, 
    ecs_size_t n, 
    char delimiter, 
    const char *in) 
{
    const char *ptr = in;
    char ch, *bptr = out, buff[3];
    ecs_size_t written = 0;
    while ((ch = *ptr++)) {
        if ((written += (ecs_size_t)(ecs_chresc(buff, ch, delimiter) - buff)) <= n) {
            *bptr++ = buff[0];
            if ((ch = buff[1])) {
                *bptr = ch;
                bptr++;
            }
        }
    }

    if (bptr) {
        while (written < n) {
            *bptr = '\0';
            bptr++;
            written++;
        }
    }
    return written;
}

/* Simple serializer to turn values into strings. Use this code as a template
 * for when implementing a new serializer. */

static
int str_ser_type(
    ecs_world_t *world,
    ecs_vector_t *ser, 
    const void *base, 
    ecs_strbuf_t *str);

static
int str_ser_type_op(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base,
    ecs_strbuf_t *str);

/* Serialize a primitive value */
static
void str_ser_primitive(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    const char *bool_str[] = { "false", "true" };

    switch(op->is.primitive) {
    case EcsBool:
        ecs_strbuf_appendstr(str, bool_str[(int)*(bool*)base]);
        break;
    case EcsChar: {
        char chbuf[3];
        ecs_chresc(chbuf, *(char*)base, '\'');

        ecs_strbuf_appendstrn(str, "'", 1);
        ecs_strbuf_appendstr(str, chbuf);
        ecs_strbuf_appendstrn(str, "'", 1);
        break;
    }
    case EcsByte:
        ecs_strbuf_append(str, "0x%x", *(uint8_t*)base);
        break;
    case EcsU8:
        ecs_strbuf_append(str, "%u", *(uint8_t*)base);
        break;
    case EcsU16:
        ecs_strbuf_append(str, "%u", *(uint16_t*)base);
        break;
    case EcsU32:
        ecs_strbuf_append(str, "%u", *(uint32_t*)base);
        break;
    case EcsU64:
        ecs_strbuf_append(str, "%llu", *(uint64_t*)base);
        break;
    case EcsI8:
        ecs_strbuf_append(str, "%d", *(int8_t*)base);
        break;
    case EcsI16:
        ecs_strbuf_append(str, "%d", *(int16_t*)base);
        break;
    case EcsI32:
        ecs_strbuf_append(str, "%d", *(int32_t*)base);
        break;
    case EcsI64:
        ecs_strbuf_append(str, "%lld", *(int64_t*)base);
        break;
    case EcsF32:
        ecs_strbuf_append(str, "%f", (double)*(float*)base);
        break;
    case EcsF64:
        ecs_strbuf_append(str, "%f", *(double*)base);
        break;
    case EcsIPtr:
        ecs_strbuf_append(str, "%i", *(intptr_t*)base);
        break;
    case EcsUPtr:
        ecs_strbuf_append(str, "%u", *(uintptr_t*)base);
        break;
    case EcsString: {
        char *value = *(char**)base;
        if (value) {
            ecs_size_t length = ecs_stresc(NULL, 0, '"', value);
            if (length == ecs_os_strlen(value)) {
                ecs_strbuf_appendstrn(str, "\"", 1);
                ecs_strbuf_appendstr(str, value);
                ecs_strbuf_appendstrn(str, "\"", 1);
            } else {
                char *out = ecs_os_malloc(length + 3);
                ecs_stresc(out + 1, length, '"', value);
                out[0] = '"';
                out[length + 1] = '"';
                out[length + 2] = '\0';
                ecs_strbuf_appendstr_zerocpy(str, out);
            }
        } else {
            ecs_strbuf_appendstr(str, "nullptr");
        }
        break;
    }
    case EcsEntity: {
        ecs_entity_t e = *(ecs_entity_t*)base;
        const char *name;
        if (e && (name = ecs_get_name(world, e))) {
            ecs_strbuf_appendstr(str, name);
        } else {
            ecs_strbuf_append(str, "%u", e);
        }

        break;
    }
    }
}

/* Serialize enumeration */
static
int str_ser_enum(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    const EcsEnum *enum_type = ecs_get_ref_w_id(world, &op->is.constant, 0, 0);
    ecs_assert(enum_type != NULL, ECS_INVALID_PARAMETER, NULL);

    int32_t value = *(int32_t*)base;
    
    /* Enumeration constants are stored in a map that is keyed on the
     * enumeration value. */
    char **constant = ecs_map_get(enum_type->constants, char*, value);
    if (!constant) {
        return -1;
    }

    ecs_strbuf_appendstr(str, *constant);

    return 0;
}

/* Serialize bitmask */
static
int str_ser_bitmask(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    const EcsBitmask *bitmask_type = ecs_get_ref_w_id(world, &op->is.constant, 0, 0);
    ecs_assert(bitmask_type != NULL, ECS_INVALID_PARAMETER, NULL);

    uint32_t value = *(uint32_t*)base;
    ecs_map_key_t key;
    char **constant;
    int count = 0;

    ecs_strbuf_list_push(str, "", " | ");

    /* Multiple flags can be set at a given time. Iterate through all the flags
     * and append the ones that are set. */
    ecs_map_iter_t it = ecs_map_iter(bitmask_type->constants);
    while ((constant = ecs_map_next(&it, char*, &key))) {
        if ((value & key) == key) {
            ecs_strbuf_list_appendstr(str, *constant);
            count ++;
        }
    }

    if (!count) {
        ecs_strbuf_list_appendstr(str, "0");
    }

    ecs_strbuf_list_pop(str, "");

    return 0;
}

/* Serialize elements of a contiguous array */
static
int str_ser_elements(
    ecs_world_t *world,
    ecs_vector_t *elem_ops, 
    const void *base, 
    int32_t elem_count, 
    int32_t elem_size,
    ecs_strbuf_t *str)
{
    ecs_strbuf_list_push(str, "[", ", ");

    const void *ptr = base;

    int i;
    for (i = 0; i < elem_count; i ++) {
        ecs_strbuf_list_next(str);
        if (str_ser_type(world, elem_ops, ptr, str)) {
            return -1;
        }
        ptr = ECS_OFFSET(ptr, elem_size);
    }

    ecs_strbuf_list_pop(str, "]");

    return 0;
}

/* Serialize array */
static
int str_ser_array(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    const EcsMetaTypeSerializer *ser = ecs_get_ref_w_id(world, &op->is.collection, 0, 0);
    ecs_assert(ser != NULL, ECS_INTERNAL_ERROR, NULL);

    return str_ser_elements(
        world, ser->ops, base, op->count, op->size, str);
}

/* Serialize vector */
static
int str_ser_vector(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    ecs_vector_t *value = *(ecs_vector_t**)base;
    if (!value) {
        ecs_strbuf_appendstr(str, "nullptr");
        return 0;
    }
    
    const EcsMetaTypeSerializer *ser = ecs_get_ref_w_id(world, &op->is.collection, 0, 0);
    ecs_assert(ser != NULL, ECS_INTERNAL_ERROR, NULL);

    int32_t count = ecs_vector_count(value);
    void *array = ecs_vector_first_t(value, op->size, op->alignment);
    ecs_vector_t *elem_ops = ser->ops;
    
    ecs_type_op_t *elem_op_hdr = (ecs_type_op_t*)ecs_vector_first(elem_ops, ecs_type_op_t);
    ecs_assert(elem_op_hdr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(elem_op_hdr->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
    ecs_size_t elem_size = elem_op_hdr->size;

    /* Serialize contiguous buffer of vector */
    return str_ser_elements(world, elem_ops, array, count, elem_size, str);
}

/* Serialize map */
static
int str_ser_map(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    ecs_map_t *value = *(ecs_map_t**)base;

    const EcsMetaTypeSerializer *key_ser = ecs_get_ref_w_id(world, &op->is.map.key, 0, 0);
    ecs_assert(key_ser != NULL, ECS_INTERNAL_ERROR, NULL);

    const EcsMetaTypeSerializer *elem_ser = ecs_get_ref_w_id(world, &op->is.map.element, 0, 0);
    ecs_assert(elem_ser != NULL, ECS_INTERNAL_ERROR, NULL);

    /* 2 instructions, one for the header */
    ecs_assert(ecs_vector_count(key_ser->ops) == 2, ECS_INTERNAL_ERROR, NULL);

    ecs_type_op_t *key_op = ecs_vector_first(key_ser->ops, ecs_type_op_t);
    ecs_assert(key_op->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
    key_op = &key_op[1];

    ecs_map_iter_t it = ecs_map_iter(value);  
    ecs_map_key_t key; 
    void *ptr;

    ecs_strbuf_list_push(str, "{", ", ");

    while ((ptr = _ecs_map_next(&it, 0, &key))) {
        ecs_strbuf_list_next(str);
        if (str_ser_type_op(world, key_op, (void*)&key, str)) {
            return -1;
        }

        ecs_strbuf_appendstr(str, " = ");
        
        if (str_ser_type(world, elem_ser->ops, ptr, str)) {
            return -1;
        }

        key = 0;
    }

    ecs_strbuf_list_pop(str, "}");

    return 0;
}

/* Forward serialization to the different type kinds */
static
int str_ser_type_op(
    ecs_world_t *world,
    ecs_type_op_t *op, 
    const void *base,
    ecs_strbuf_t *str) 
{
    switch(op->kind) {
    case EcsOpHeader:
    case EcsOpPush:
    case EcsOpPop:
        /* Should not be parsed as single op */
        ecs_abort(ECS_INVALID_PARAMETER, NULL);
        break;
    case EcsOpPrimitive:
        str_ser_primitive(world, op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpEnum:
        if (str_ser_enum(world, op, ECS_OFFSET(base, op->offset), str)) {
            return -1;
        }
        break;
    case EcsOpBitmask:
        if (str_ser_bitmask(world, op, ECS_OFFSET(base, op->offset), str)) {
            return -1;
        }
        break;
    case EcsOpArray:
        if (str_ser_array(world, op, ECS_OFFSET(base, op->offset), str)) {
            return -1;
        }
        break;
    case EcsOpVector:
        if (str_ser_vector(world, op, ECS_OFFSET(base, op->offset), str)) {
            return -1;
        }
        break;
    case EcsOpMap:
        if (str_ser_map(world, op, ECS_OFFSET(base, op->offset), str)) {
            return -1;
        }
        break;
    }

    return 0;
}

/* Iterate over the type ops of a type */
static
int str_ser_type(
    ecs_world_t *world,
    ecs_vector_t *ser, 
    const void *base, 
    ecs_strbuf_t *str) 
{
    ecs_type_op_t *ops = (ecs_type_op_t*)ecs_vector_first(ser, ecs_type_op_t);
    int32_t count = ecs_vector_count(ser);

    for (int i = 0; i < count; i ++) {
        ecs_type_op_t *op = &ops[i];

        if (op->name) {
            if (op->kind != EcsOpHeader)
            {
                ecs_strbuf_list_next(str);
            }

            ecs_strbuf_append(str, "%s = ", op->name);
        }

        switch(op->kind) {
        case EcsOpHeader:
            break;
        case EcsOpPush:
            ecs_strbuf_list_push(str, "{", ", ");
            break;
        case EcsOpPop:
            ecs_strbuf_list_pop(str, "}");
            break;
        default:
            if (str_ser_type_op(world, op, base, str)) {
                goto error;
            }
            break;
        }
    }

    return 0;
error:
    ecs_strbuf_reset(str);
    return -1;
}

char* ecs_ptr_to_str(
    ecs_world_t *world, 
    ecs_entity_t type, 
    void* ptr)
{
    const EcsMetaTypeSerializer *ser = ecs_get(world, type, EcsMetaTypeSerializer);
    ecs_assert(ser != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_strbuf_t str = ECS_STRBUF_INIT;
    if (str_ser_type(world, ser->ops, ptr, &str)) {
        return NULL;
    }

    return ecs_strbuf_get(&str);
}

char* ecs_entity_to_str(
    ecs_world_t *world, 
    ecs_entity_t entity)
{
    ecs_type_t type = ecs_get_type(world, entity);
    ecs_entity_t *ids = (ecs_entity_t*)ecs_vector_first(type, ecs_entity_t);
    int32_t count = ecs_vector_count(type);
    
    ecs_strbuf_t str = ECS_STRBUF_INIT;

    const char *name = ecs_get_name(world, entity);
    if (name) {
        ecs_strbuf_append(&str, "%s: ", name);
    }

    ecs_strbuf_appendstr(&str, "{\n");

    int i, comps_serialized = 0;
    for (i = 0; i < count; i ++) {
        const EcsMetaTypeSerializer *ser = ecs_get(world, ids[i], EcsMetaTypeSerializer);
        if (ser) {
            const void *ptr = ecs_get_id(world, entity, ids[i]);
            ecs_strbuf_append(&str, "    %s: ", ecs_get_name(world, ids[i]));
            if (str_ser_type(world, ser->ops, ptr, &str)) {
                goto error;
            }

            ecs_strbuf_appendstr(&str, "\n");
            comps_serialized ++;
        }
    }

    ecs_strbuf_appendstr(&str, "}");

    return ecs_strbuf_get(&str);  
error:
    ecs_strbuf_reset(&str);
    return NULL;
}
#ifndef FLECS_META_TYPE_H
#define FLECS_META_TYPE_H


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

ecs_entity_t ecs_meta_lookup_bitmask(
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

ECS_COMPONENT_DECLARE(EcsPrimitive);
ECS_COMPONENT_DECLARE(EcsEnum);
ECS_COMPONENT_DECLARE(EcsBitmask);
ECS_COMPONENT_DECLARE(EcsMember);
ECS_COMPONENT_DECLARE(EcsStruct);
ECS_COMPONENT_DECLARE(EcsArray);
ECS_COMPONENT_DECLARE(EcsVector);
ECS_COMPONENT_DECLARE(EcsMap);
ECS_COMPONENT_DECLARE(EcsMetaType);
ECS_COMPONENT_DECLARE(ecs_type_op_kind_t);
ECS_COMPONENT_DECLARE(ecs_type_op_t);
ECS_COMPONENT_DECLARE(EcsMetaTypeSerializer);

static ECS_CTOR(EcsMetaType, ptr, {
    ptr->descriptor = NULL;
    ptr->alias = NULL;
})

static ECS_DTOR(EcsMetaType, ptr, {
    ecs_os_free((char*)ptr->descriptor);
    ptr->descriptor = NULL;
    ptr->alias = NULL;
})

static ECS_COPY(EcsMetaType, dst, src, {
    ecs_os_free((char*)dst->descriptor);

    dst->descriptor = ecs_os_strdup(src->descriptor);

    dst->kind = src->kind;
    dst->size = src->size;
    dst->alignment = src->alignment;
    dst->alias = src->alias;
})

static ECS_MOVE(EcsMetaType, dst, src, {
    ecs_os_free((char*)dst->descriptor);

    dst->kind = src->kind;
    dst->size = src->size;
    dst->alignment = src->alignment;
    dst->descriptor = src->descriptor;
    dst->alias = src->alias;

    src->descriptor = NULL;
    src->alias = NULL;
})

static ECS_CTOR(EcsStruct, ptr, {
    ptr->members = NULL;
    ptr->is_partial = false;
})

static ECS_DTOR(EcsStruct, ptr, {
    ecs_vector_each(ptr->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(ptr->members);
})

static ECS_COPY(EcsStruct, dst, src, {
    ecs_vector_each(dst->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(dst->members);

    dst->members = ecs_vector_copy(src->members, EcsMember);
    EcsMember *src_struct = ecs_vector_first(src->members, EcsMember);

    ecs_vector_each(dst->members, EcsMember, m, {
        m->name = ecs_os_strdup(src_struct->name);
        src_struct++;
    });
    dst->is_partial = src->is_partial;
})

static ECS_MOVE(EcsStruct, dst, src, {
    ecs_vector_each(dst->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(dst->members);

    dst->members = src->members;
    dst->is_partial = src->is_partial;

    src->members = NULL;
})

static ECS_CTOR(EcsEnum, ptr, {
    ptr->constants = NULL;
})

static ECS_DTOR(EcsEnum, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(ptr->constants);

    ptr->constants = NULL;
})

static ECS_COPY(EcsEnum, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    int32_t count = ecs_map_count(src->constants);
    dst->constants = ecs_map_new(char*, count);

    ecs_map_each(src->constants, char*, key, c_ptr, {
        char *constant_name = ecs_os_strdup(*c_ptr);
        ecs_map_set(dst->constants, key, &constant_name);
    })
})

static ECS_MOVE(EcsEnum, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    dst->constants = src->constants;
    src->constants = NULL;
})

static ECS_CTOR(EcsBitmask, ptr, {
    ptr->constants = NULL;
})

static ECS_DTOR(EcsBitmask, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(ptr->constants);

    ptr->constants = NULL;
})

static ECS_COPY(EcsBitmask, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    int32_t count = ecs_map_count(src->constants);
    dst->constants = ecs_map_new(char*, count);

    ecs_map_each(src->constants, char*, key, c_ptr, {
        char *constant_name = ecs_os_strdup(*c_ptr);
        ecs_map_set(dst->constants, key, &constant_name);
    })
})

static ECS_MOVE(EcsBitmask, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    dst->constants = src->constants;
    src->constants = NULL;
})

static ECS_CTOR(EcsMetaTypeSerializer, ptr, {
    ptr->ops = NULL;
})

static ECS_DTOR(EcsMetaTypeSerializer, ptr, {
    ecs_vector_free(ptr->ops);
    ptr->ops = NULL;
})

static ECS_COPY(EcsMetaTypeSerializer, dst, src, {
    ecs_vector_free(dst->ops);
    dst->ops = ecs_vector_copy(src->ops, ecs_type_op_t);
})

static ECS_MOVE(EcsMetaTypeSerializer, dst, src, {
    ecs_vector_free(dst->ops);

    dst->ops = src->ops;
    src->ops = NULL;
})

static
void ecs_set_primitive(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *descr = type->descriptor;

    if (!strcmp(descr, "bool")) {
        ecs_set(world, e, EcsPrimitive, {EcsBool});
    } else
    if (!strcmp(descr, "char")) {
        ecs_set(world, e, EcsPrimitive, {EcsChar});
    } else
    if (!strcmp(descr, "u8")) {
        ecs_set(world, e, EcsPrimitive, {EcsU8});
    } else
    if (!strcmp(descr, "u8")) {
        ecs_set(world, e, EcsPrimitive, {EcsU8});
    } else
    if (!strcmp(descr, "u16")) {
        ecs_set(world, e, EcsPrimitive, {EcsU16});
    } else
    if (!strcmp(descr, "u32")) {
        ecs_set(world, e, EcsPrimitive, {EcsU32});
    } else
    if (!strcmp(descr, "u64")) {
        ecs_set(world, e, EcsPrimitive, {EcsU64});
    } else
    if (!strcmp(descr, "i8")) {
        ecs_set(world, e, EcsPrimitive, {EcsI8});
    } else
    if (!strcmp(descr, "i16")) {
        ecs_set(world, e, EcsPrimitive, {EcsI16});
    } else
    if (!strcmp(descr, "i32")) {
        ecs_set(world, e, EcsPrimitive, {EcsI32});
    } else
    if (!strcmp(descr, "f32")) {
        ecs_set(world, e, EcsPrimitive, {EcsF32});
    } else
    if (!strcmp(descr, "f64")) {
        ecs_set(world, e, EcsPrimitive, {EcsF64});
    } else
    if (!strcmp(descr, "iptr")) {
        ecs_set(world, e, EcsPrimitive, {EcsIPtr});
    } else
    if (!strcmp(descr, "uptr")) {
        ecs_set(world, e, EcsPrimitive, {EcsUPtr});
    } else
    if (!strcmp(descr, "string_t")) {
        ecs_set(world, e, EcsPrimitive, {EcsString});
    } else
    if (!strcmp(descr, "entity")) {
        ecs_set(world, e, EcsPrimitive, {EcsEntity});
    }
}

static
void ecs_set_constants(
    ecs_world_t *world,
    ecs_entity_t e,
    ecs_entity_t comp,
    bool is_bitmask,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_map_t *constants = ecs_map_new(char*, 1);
    ecs_meta_constant_t token;
    int64_t last_value = 0;

    while ((ptr = ecs_meta_parse_constant(ptr, &token, &ctx))) {
        if (token.is_value_set) {
            last_value = token.value;
        } else if (is_bitmask) {
            ecs_meta_error(&ctx, ptr,
                "bitmask requires explicit value assignment");
        }

        char *constant_name = ecs_os_strdup(token.name);
        ecs_map_set(constants, last_value, &constant_name);

        last_value ++;
    }

    ecs_set_id(world, e, comp, sizeof(EcsEnum), &(EcsEnum){
        .constants = constants
    });
}

static
void ecs_set_bitmask(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_set_constants(world, e, ecs_id(EcsBitmask), true, type);
}

static
void ecs_set_enum(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_set_constants(world, e, ecs_id(EcsEnum), false, type);
}

static
void ecs_set_struct(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);
    bool is_partial = false;

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_vector_t *members = NULL;
    ecs_meta_member_t token;

    while ((ptr = ecs_meta_parse_member(ptr, &token, &ctx))) {
        EcsMember *m = ecs_vector_add(&members, EcsMember);
        m->name = ecs_os_strdup(token.name);
        m->type = ecs_meta_lookup(world, &token.type, ptr, token.count, &ctx);
        ecs_assert(type != 0, ECS_INTERNAL_ERROR, NULL);
    }

    if (!ecs_vector_count(members)) {
        ecs_parser_error(name, type->descriptor, 0, "empty struct declaration");
    }

    is_partial = token.is_partial;

    ecs_set(world, e, EcsStruct, {members, is_partial});
}

static
void ecs_set_array(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_array(world, e, type->descriptor, &ctx);
}

static
void ecs_set_vector(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_vector(world, e, type->descriptor, &ctx);
}

static
void ecs_set_map(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_map(world, e, type->descriptor, &ctx);
}

static
void EcsSetType(ecs_iter_t *it) {
    EcsMetaType *type = ecs_term(it, EcsMetaType, 1);

    ecs_world_t *world = it->world;

    int i;
    for(i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

        /* If type does not contain a descriptor, application will have to
         * manually initialize type specific data */
        if (!type[i].descriptor) {

            /* For some types we can set the size and alignment automatically */
            if (!type[i].size || !type[i].alignment) {
                switch(type[i].kind) {
                case EcsBitmaskType:
                case EcsEnumType:
                    type[i].size = sizeof(int32_t);
                    type[i].alignment = ECS_ALIGNOF(int32_t);
                    break;
                case EcsVectorType:
                    type[i].size = sizeof(ecs_vector_t*);
                    type[i].alignment = ECS_ALIGNOF(ecs_vector_t*);
                    break;
                case EcsMapType:
                    type[i].size = sizeof(ecs_map_t*);
                    type[i].alignment = ECS_ALIGNOF(ecs_map_t*);
                    break;
                default:
                    break;
                }
            }

            continue;
        }

        switch(type[i].kind) {
        case EcsPrimitiveType:
            ecs_set_primitive(world, e, type);
            break;
        case EcsBitmaskType:
            ecs_set_bitmask(world, e, type);
            break;
        case EcsEnumType:
            ecs_set_enum(world, e, type);
            break;
        case EcsStructType:
            ecs_set_struct(world, e, type);
            break;
        case EcsArrayType:
            ecs_set_array(world, e, type);
            break;
        case EcsVectorType:
            ecs_set_vector(world, e, type);
            break;
        case EcsMapType:
            ecs_set_map(world, e, type);
            break;
        }
    }
}

void ecs_new_meta(
    ecs_world_t *world,
    ecs_entity_t component,
    EcsMetaType *meta_type)
{
    ecs_assert(ecs_id(EcsMetaType) != 0, ECS_MODULE_UNDEFINED, "flecs.meta");

    if (meta_type->alias) {
        EcsMetaType *alias = meta_type->alias;
        meta_type->kind = alias->kind;
        meta_type->descriptor = alias->descriptor;
    }

    ecs_set_ptr(world, component, EcsMetaType, meta_type);
}

/* Utility macro to insert meta data for type with meta descriptor */
#define ECS_COMPONENT_TYPE(world, type)\
    ecs_set_ptr(world, ecs_id(type), EcsMetaType, &ecs_meta(type))

/* Utility macro to insert metadata for primitive type */
#define ECS_COMPONENT_PRIMITIVE(world, type, kind)\
    ECS_COMPONENT(world, type);\
    ecs_set(world, ecs_id(type), EcsMetaType, {EcsPrimitiveType, 0, 0, NULL, NULL});\
    ecs_set(world, ecs_id(type), EcsPrimitive, {kind})

void FlecsMetaImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsMeta);

    ecs_set_name_prefix(world, "Ecs");

    ECS_COMPONENT_DEFINE(world, EcsPrimitive);
    ECS_COMPONENT_DEFINE(world, EcsEnum);
    ECS_COMPONENT_DEFINE(world, EcsBitmask);
    ECS_COMPONENT_DEFINE(world, EcsMember);
    ECS_COMPONENT_DEFINE(world, EcsStruct);
    ECS_COMPONENT_DEFINE(world, EcsArray);
    ECS_COMPONENT_DEFINE(world, EcsVector);
    ECS_COMPONENT_DEFINE(world, EcsMap);
    ECS_COMPONENT_DEFINE(world, EcsMetaType);
    ECS_COMPONENT_DEFINE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_DEFINE(world, ecs_type_op_t);
    ECS_COMPONENT_DEFINE(world, EcsMetaTypeSerializer);

    ECS_OBSERVER(world, EcsSetType, EcsOnSet, EcsMetaType);

    ecs_set_component_actions(world, EcsMetaType, {
        .ctor = ecs_ctor(EcsMetaType),
        .dtor = ecs_dtor(EcsMetaType),
        .copy = ecs_copy(EcsMetaType),
        .move = ecs_move(EcsMetaType)
    });

    ecs_set_component_actions(world, EcsStruct, {
        .ctor = ecs_ctor(EcsStruct),
        .dtor = ecs_dtor(EcsStruct),
        .copy = ecs_copy(EcsStruct),
        .move = ecs_move(EcsStruct)
    });

    ecs_set_component_actions(world, EcsEnum, {
        .ctor = ecs_ctor(EcsEnum),
        .dtor = ecs_dtor(EcsEnum),
        .copy = ecs_copy(EcsEnum),
        .move = ecs_move(EcsEnum)
    });

    ecs_set_component_actions(world, EcsBitmask, {
        .ctor = ecs_ctor(EcsBitmask),
        .dtor = ecs_dtor(EcsBitmask),
        .copy = ecs_copy(EcsBitmask),
        .move = ecs_move(EcsBitmask)
    });

    ecs_set_component_actions(world, EcsMetaTypeSerializer, {
        .ctor = ecs_ctor(EcsMetaTypeSerializer),
        .dtor = ecs_dtor(EcsMetaTypeSerializer),
        .copy = ecs_copy(EcsMetaTypeSerializer),
        .move = ecs_move(EcsMetaTypeSerializer)
    });

    ECS_OBSERVER(world, EcsSetPrimitive, EcsOnSet, Primitive);
    ECS_OBSERVER(world, EcsSetEnum, EcsOnSet, Enum);
    ECS_OBSERVER(world, EcsSetBitmask, EcsOnSet, Bitmask);
    ECS_OBSERVER(world, EcsSetStruct, EcsOnSet, Struct);
    ECS_OBSERVER(world, EcsSetArray, EcsOnSet, Array);
    ECS_OBSERVER(world, EcsSetVector, EcsOnSet, Vector);
    ECS_OBSERVER(world, EcsSetMap, EcsOnSet, Map);

    ECS_EXPORT_COMPONENT(EcsPrimitive);
    ECS_EXPORT_COMPONENT(EcsEnum);
    ECS_EXPORT_COMPONENT(EcsBitmask);
    ECS_EXPORT_COMPONENT(EcsStruct);
    ECS_EXPORT_COMPONENT(EcsArray);
    ECS_EXPORT_COMPONENT(EcsVector);
    ECS_EXPORT_COMPONENT(EcsMap);
    ECS_EXPORT_COMPONENT(EcsMetaType);
    ECS_EXPORT_COMPONENT(EcsMetaTypeSerializer);

    /* -- Initialize builtin primitive types -- */
    ecs_entity_t old_scope = ecs_set_scope(world, EcsFlecsCore);
    ECS_COMPONENT_PRIMITIVE(world, bool, EcsBool);
    ECS_COMPONENT_PRIMITIVE(world, char, EcsChar);
    ECS_COMPONENT_PRIMITIVE(world, ecs_byte_t, EcsByte);
    ECS_COMPONENT_PRIMITIVE(world, uint8_t, EcsU8);
    ECS_COMPONENT_PRIMITIVE(world, uint16_t, EcsU16);
    ECS_COMPONENT_PRIMITIVE(world, uint32_t, EcsU32);
    ECS_COMPONENT_PRIMITIVE(world, uint64_t, EcsU64);
    ECS_COMPONENT_PRIMITIVE(world, uintptr_t, EcsUPtr);
    ECS_COMPONENT_PRIMITIVE(world, int8_t, EcsI8);
    ECS_COMPONENT_PRIMITIVE(world, int16_t, EcsI16);
    ECS_COMPONENT_PRIMITIVE(world, int32_t, EcsI32);
    ECS_COMPONENT_PRIMITIVE(world, int64_t, EcsI64);
    ECS_COMPONENT_PRIMITIVE(world, intptr_t, EcsIPtr);
    ECS_COMPONENT_PRIMITIVE(world, size_t, EcsUPtr);
    ECS_COMPONENT_PRIMITIVE(world, float, EcsF32);
    ECS_COMPONENT_PRIMITIVE(world, double, EcsF64);
    ECS_COMPONENT_PRIMITIVE(world, ecs_size_t, EcsI32);
    ECS_COMPONENT_PRIMITIVE(world, ecs_string_t, EcsString);
    ECS_COMPONENT_PRIMITIVE(world, ecs_entity_t, EcsEntity);

    /* If stdbool is included, the above bool declaration will have been
     * registered with the name _Bool. To make sure meta also knows the type by
     * its regular name, check and register if necessary */
    if (!ecs_lookup(world, "bool")) {
        ecs_entity_t type = ecs_component_init(world, &(ecs_component_desc_t) {
            .entity.name = "bool",
            .size = sizeof(bool),
            .alignment = ECS_ALIGNOF(bool)
        });

        ecs_set(world, type, EcsMetaType, {
            EcsPrimitiveType, 0, 0, NULL, NULL});
        ecs_set(world, type, EcsPrimitive, {EcsBool});
    }

    ecs_set_scope(world, old_scope);

    /* -- Initialize builtin meta components -- */

    ecs_set_ptr(world, ecs_set_name(world, 0, "ecs_primitive_kind_t"),
        EcsMetaType, &ecs_meta(ecs_primitive_kind_t));

    ecs_set(world, ecs_set_name(world, 0, "ecs_type_kind_t"),
        EcsMetaType, {
            EcsEnumType,
            sizeof(ecs_type_kind_t),
            ECS_ALIGNOF(ecs_type_kind_t),
            ecs_meta(ecs_type_kind_t),
            NULL
        });

    /* Insert meta definitions for other types */
    ECS_COMPONENT_TYPE(world, EcsPrimitive);
    ECS_COMPONENT_TYPE(world, EcsEnum);
    ECS_COMPONENT_TYPE(world, EcsBitmask);
    ECS_COMPONENT_TYPE(world, EcsMember);
    ECS_COMPONENT_TYPE(world, EcsStruct);
    ECS_COMPONENT_TYPE(world, EcsArray);
    ECS_COMPONENT_TYPE(world, EcsVector);
    ECS_COMPONENT_TYPE(world, EcsMap);
    ECS_COMPONENT_TYPE(world, EcsMetaType);
    ECS_COMPONENT_TYPE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_TYPE(world, ecs_type_op_t);
    ECS_COMPONENT_TYPE(world, EcsMetaTypeSerializer);

    /* -- Initialize metadata for public Flecs core components -- */
    ecs_set(world, ecs_id(EcsIdentifier), EcsMetaType, {
        .kind = EcsStructType,
        .size = sizeof(EcsIdentifier),
        .alignment = ECS_ALIGNOF(EcsIdentifier),
        .descriptor = "{ char *value; ECS_PRIVATE }",
        .alias = NULL
    });

    ecs_set(world, ecs_id(EcsComponent), EcsMetaType, {
        .kind = EcsStructType,
        .size = sizeof(EcsComponent),
        .alignment = ECS_ALIGNOF(EcsComponent),
        .descriptor = "{int32_t size; int32_t alignment;}",
        .alias = NULL
    });
}

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

    ecs_entity_t element_type = ecs_lookup_symbol(world, params.type.type, true);
    if (!element_type) {
        ecs_meta_error(ctx, params_decl, "unknown element type '%s'",
            params.type.type);
    }

    if (!e) {
        const EcsMetaType *elem_type = ecs_get(world, element_type, EcsMetaType);
        ecs_assert(elem_type != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_assert(elem_type->size * params.count <= INT32_MAX,
            ECS_INVALID_PARAMETER, NULL);

        e = ecs_set(world, 0, EcsMetaType, {
            EcsArrayType, (int32_t)(elem_type->size * params.count),
            elem_type->alignment, NULL, NULL
        });
    }

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
        e = ecs_set(world, 0, EcsMetaType, {EcsVectorType, 0, 0, NULL, NULL});
    }

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
        e = ecs_set(world, 0, EcsMetaType, {EcsMapType, 0, 0, NULL, NULL});
    }

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

#ifndef NDEBUG
    /* Make sure this is a bitmask type */
    const EcsMetaType *type_ptr = ecs_get(world, bitmask_type, EcsMetaType);
    ecs_assert(type_ptr != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(type_ptr->kind == EcsBitmaskType, ECS_INVALID_PARAMETER, NULL);
#endif

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

        type = ecs_lookup_symbol(world, typename, true);
        if (!type) {
            ecs_meta_error(ctx, ptr, "unknown type '%s'", typename);
            return 0;
        }
    }

    if (count != 1) {
        /* If count is not 1, insert array type. First lookup EcsMetaType of the
         * element type to get the size and alignment. Then create a new
         * entity for the array type, and assign it to the member type. */
        const EcsMetaType *type_ptr = ecs_get(world, type, EcsMetaType);

        ecs_assert(count <= INT32_MAX, ECS_INVALID_PARAMETER, NULL);

        type = ecs_set(world, ecs_set(world, 0,
            EcsMetaType, {EcsArrayType, type_ptr->size, type_ptr->alignment, NULL, NULL}),
            EcsArray, {type, (int32_t)count});
    }

    return type;
}

static
ecs_meta_scope_t* get_scope(
    ecs_meta_cursor_t *cursor)
{
    ecs_assert(cursor != NULL, ECS_INVALID_PARAMETER, NULL);
    return &cursor->scope[cursor->depth];
}

static
ecs_type_op_t* get_op(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    return &ops[scope->cur_op];
}

static
ecs_type_op_t* get_ptr(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *op = get_op(scope);

    if (scope->vector) {
        _ecs_vector_set_min_count(&scope->vector, ECS_VECTOR_U(op->size, op->alignment), scope->cur_elem + 1);
        scope->base = ecs_vector_first_t(scope->vector, op->size, op->alignment);
    }

    return ECS_OFFSET(scope->base, op->offset + op->size * scope->cur_elem);
}

ecs_meta_cursor_t ecs_meta_cursor(
    const ecs_world_t *world,
    ecs_entity_t type,
    void *base)
{
    ecs_assert(world != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(type != 0, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(base != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_meta_cursor_t result;

    const EcsMetaTypeSerializer *ser = ecs_get(world, type, EcsMetaTypeSerializer);
    ecs_assert(ser != NULL, ECS_INVALID_PARAMETER, NULL);

#ifndef NDEBUG
    ecs_type_op_t *ops = ecs_vector_first(ser->ops, ecs_type_op_t);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(ops[0].kind == EcsOpHeader, ECS_INVALID_PARAMETER, NULL);
#endif

    result.world = world;
    result.depth = 0;
    result.scope[0].type = type;
    result.scope[0].ops = ser->ops;
    result.scope[0].start = 1;
    result.scope[0].cur_op = 1;
    result.scope[0].cur_elem = 0;
    result.scope[0].base = base;
    result.scope[0].is_collection = false;
    result.scope[0].count = 0;
    result.scope[0].vector = NULL;

    return result;
}

void* ecs_meta_get_ptr(
    ecs_meta_cursor_t *cursor)
{
    return get_ptr(cursor->scope);
}

int ecs_meta_next(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    int32_t ops_count = ecs_vector_count(scope->ops);

    if (scope->count) {
        if (scope->cur_op >= scope->count) {
            return -1;
        }
    } else {
        if (scope->cur_op >= ops_count) {
            return -1;
        }
    }

    if (scope->is_collection) {
        scope->cur_op = 1;
        scope->cur_elem ++;

        if (scope->count) {
            if (scope->cur_elem >= scope->count) {
                return -1;
            }
        }
    } else {
        scope->cur_op ++;
    }

    return 0;
}

int ecs_meta_move(
    ecs_meta_cursor_t *cursor,
    int32_t pos)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    int32_t ops_count = ecs_vector_count(scope->ops);

    if(pos < 0) return -1;

    if (scope->is_collection) {
        if (scope->count) {
            if (pos >= scope->count) {
                return -1;
            }

            scope->cur_op = 1;
            scope->cur_elem = pos;
        }
    } else {
        if (pos >= ops_count) {
            return -1;
        }
        scope->cur_op = scope->start + pos;
    }

    return 0;
}

int ecs_meta_move_name(
    ecs_meta_cursor_t *cursor,
    const char *name)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    int32_t i, ops_count = ecs_vector_count(scope->ops);
    int32_t depth = 1;

    for (i = scope->start; i < ops_count; i ++) {
        ecs_type_op_t *op = &ops[i];

        if (depth <= 1) {
            if (op->name && !strcmp(op->name, name)) {
                scope->cur_op = i;
                return 0;
            }
        }

        if (op->kind == EcsOpPush) {
            depth ++;
        }

        if (op->kind == EcsOpPop) {
            depth --;
            if (depth < 0) {
                return -1;
            }
        }
    }

    return -1;
}

int ecs_meta_push(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (scope->vector) {
        /* This makes sure the vector has enough space for the pushed element */
        get_ptr(scope);
    }

    scope->cur_op ++;
    cursor->depth ++;
    ecs_meta_scope_t *child_scope = get_scope(cursor);
    child_scope->cur_elem = 0;

    switch(op->kind) {
    case EcsOpPush: {
        child_scope->base = ECS_OFFSET(scope->base, op->size * scope->cur_elem);
        child_scope->start = scope->cur_op;
        child_scope->cur_op = scope->cur_op;
        child_scope->ops = scope->ops;
        child_scope->is_collection = false;
        child_scope->count = 0;
        child_scope->vector = NULL;
        break;
    }
    case EcsOpArray:
    case EcsOpVector: {
        void *ptr = ECS_OFFSET(scope->base, op->offset);
        const EcsMetaTypeSerializer *ser = ecs_get_ref_w_id(cursor->world,
            &op->is.collection, 0, 0);
        ecs_assert(ser != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_vector_t *ops = ser->ops;

        if (op->kind == EcsOpArray) {
            child_scope->base = ptr;
            child_scope->count = op->count;
            child_scope->vector = NULL;
        } else {
            ecs_vector_t *v = *(ecs_vector_t**)ptr;
            if (!v) {
                v = ecs_vector_new_t(op->size, op->alignment, 2);
            } else {
                ecs_vector_set_count_t(&v, op->size, op->alignment, 0);
            }

            child_scope->base = ecs_vector_first_t(v, op->size, op->alignment);
            child_scope->count = 0;
            child_scope->vector = v;
        }
        child_scope->start = 1;
        child_scope->cur_op = 1;
        child_scope->ops = ops;
        child_scope->is_collection = true;
#ifndef NDEBUG
        ecs_type_op_t *hdr = ecs_vector_first(child_scope->ops, ecs_type_op_t);
        ecs_assert(hdr->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
#endif
        }
        break;
    default:
        return -1;
    }

    return 0;
}

int ecs_meta_pop(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    int32_t i, ops_count = ecs_vector_count(scope->ops);

    if (scope->is_collection) {
        cursor->depth --;
        if (scope->vector) {
            /* Vector ptr may have changed, so reassign vector field */
            ecs_meta_scope_t *parent_scope = get_scope(cursor);
            parent_scope->cur_op --;
            void *ptr = get_ptr(parent_scope);
            *(ecs_vector_t**)ptr = scope->vector;
            parent_scope->cur_op ++;
        }
        return 0;
    } else {
        for (i = scope->cur_op; i < ops_count; i ++) {
            ecs_type_op_t *op = &ops[i];
            if (op->kind == EcsOpPop) {
                cursor->depth -- ;
                ecs_meta_scope_t *parent_scope = get_scope(cursor);
                if (parent_scope->is_collection) {
                    parent_scope->cur_op = 1;
                    parent_scope->cur_elem ++;
                } else {
                    parent_scope->cur_op = i;
                }
                return 0;
            }
        }
    }

    return -1;
}

int ecs_meta_set_bool(
    ecs_meta_cursor_t *cursor,
    bool value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive || op->is.primitive != EcsBool) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);
        *(bool*)ptr = value;
        return 0;
    }
}

int ecs_meta_set_char(
    ecs_meta_cursor_t *cursor,
    char value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive || op->is.primitive != EcsChar) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);
        *(char*)ptr = value;
        return 0;
    }
}

int ecs_meta_set_int(
    ecs_meta_cursor_t *cursor,
    int64_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    ecs_primitive_kind_t primitive = op->is.primitive;

    if (op->kind != EcsOpPrimitive) {
        if (op->kind == EcsOpEnum || op->kind == EcsOpBitmask) {
            primitive = EcsI32;
        } else {
            return -1;
        }
    }

    void *ptr = get_ptr(scope);

    switch(primitive) {
    case EcsBool:
        if (value > 1 || value < 0) {
            return -1;
        }
        *(bool*)ptr = (bool)value;
        break;
    case EcsI8:
    case EcsChar:
        if (value > INT8_MAX || value < INT8_MIN) {
            return -1;
        }
        *(int8_t*)ptr = (int8_t)value;
        break;
    case EcsU8:
    case EcsByte:
        if (value > UINT8_MAX || value < INT8_MIN) {
            return -1;
        }
        *(uint8_t*)ptr = (uint8_t)value;
        break;
    case EcsI16:
        if (value > INT16_MAX || value < INT16_MIN) {
            return -1;
        }
        *(int16_t*)ptr = (int16_t)value;
        break;
    case EcsU16:
        if (value > UINT16_MAX || value < INT16_MIN) {
            return -1;
        }
        *(uint16_t*)ptr = (uint16_t)value;
        break;
    case EcsI32:
        if (value > INT32_MAX || value < INT32_MIN) {
            return -1;
        }
        *(int32_t*)ptr = (int32_t)value;
        break;
    case EcsU32:
        if (value > UINT32_MAX || value < INT32_MIN) {
            return -1;
        }
        *(uint32_t*)ptr = (uint32_t)value;
        break;
    case EcsI64:
        if (value > INT64_MAX) {
            return -1;
        }
        *(int64_t*)ptr = (int64_t)value;
        break;
    case EcsU64:
        *(uint64_t*)ptr = (uint64_t)value;
        break;
    case EcsEntity:
        *(ecs_entity_t*)ptr = (ecs_entity_t)value;
        break;
    case EcsF32:
        if (value > ((1 << 24)-1) || value < -(1 << 24)) {
            return -1;
        }
        *(float*)ptr = (float)value;
        break;
    case EcsF64:
        if (value > ((1LL << 53)-1) || value < -(1LL << 53)) {
            return -1;
        }
        *(double*)ptr = (double)value;
        break;
    case EcsIPtr:
        if (value > INTPTR_MAX) {
            return -1;
        }
        *(intptr_t*)ptr = (intptr_t)value;
        break;
    case EcsUPtr:
        *(uintptr_t*)ptr = (uintptr_t)value;
        break;
    default:
        if(!value) return ecs_meta_set_null(cursor);
        return -1;
    }

    return 0;
}

int ecs_meta_set_uint(
    ecs_meta_cursor_t *cursor,
    uint64_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsU8:
        case EcsByte:
            if (value > UINT8_MAX) {
                return -1;
            }
            *(uint8_t*)ptr = (uint8_t)value;
            break;
        case EcsU16:
            if (value > UINT16_MAX) {
                return -1;
            }
            *(uint16_t*)ptr = (uint16_t)value;
            break;
        case EcsU32:
            if (value > UINT32_MAX) {
                return -1;
            }
            *(uint32_t*)ptr = (uint32_t)value;
            break;
        case EcsU64:
            if (value > UINT64_MAX) {
                return -1;
            }
            *(uint64_t*)ptr = (uint64_t)value;
            break;
        case EcsUPtr:
            if (value > UINTPTR_MAX) {
                return -1;
            }
            *(uintptr_t*)ptr = (uintptr_t)value;
            break;
        case EcsEntity:
            *(ecs_entity_t*)ptr = value;
            break;
        default:
            if(!value) return ecs_meta_set_null(cursor);
            return -1;
        }

        return 0;
    }
}

int ecs_meta_set_float(
    ecs_meta_cursor_t *cursor,
    double value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsF32:
            *(float*)ptr = (float)value;
            break;
        case EcsF64:
            *(double*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_string(
    ecs_meta_cursor_t *cursor,
    const char *value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsString:
            if (*(char**)ptr) {
                ecs_os_free(*(char**)ptr);
            }
            if (value) {
                *(char**)ptr = ecs_os_strdup(value);
            } else {
                *(char**)ptr = NULL;
            }
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_entity(
    ecs_meta_cursor_t *cursor,
    ecs_entity_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsEntity:
            *(ecs_entity_t*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_null(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    switch(op->kind) {
    case EcsOpPrimitive: {
        if (op->is.primitive != EcsString) {
            return -1;
        }

        void *ptr = get_ptr(scope);
        char *str = *(char**)ptr;
        if (str) {
            ecs_os_free(str);
        }

        *(char**)ptr = NULL;
        break;
    }

    case EcsOpVector: {
        void *ptr = get_ptr(scope);
        ecs_vector_t *vec = *(ecs_vector_t**)ptr;
        if (vec) {
            ecs_vector_free(vec);
        }

        *(ecs_vector_t**)ptr = NULL;
        break;
    }

    default:
        return -1;
        break;
    }

    return 0;
}
#include <stdio.h>
#include <ctype.h>

static
const char* skip_ws(const char *ptr) {
    while (isspace(*ptr)) {
        ptr ++;
    }

    return ptr;
}

static
const char* skip_scope(const char *ptr, ecs_meta_parse_ctx_t *ctx) {
    /* Keep track of which characters were used to open the scope */
    char stack[256];
    int32_t sp = 0;
    char ch;

    while ((ch = *ptr)) {
        if (ch == '(') {
            stack[sp++] = ch;
        } else if (ch == '<') {
            stack[sp++] = ch;
        } else if (ch == '>') {
            if (stack[--sp] != '<') {
                ecs_meta_error(ctx, ptr, "mismatching < > in type definition");
            }
        } else if (ch == ')') {
            if (stack[--sp] != '(') {
                ecs_meta_error(ctx, ptr, "mismatching ( ) in type definition");
            }            
        }

        ptr ++;

        if (!sp) {
            break;
        }
    }

    return ptr;
}

static
const char* parse_digit(
    const char *ptr,
    int64_t *value_out,
    ecs_meta_parse_ctx_t *ctx)
{
    ptr = skip_ws(ptr);

    if (!isdigit(*ptr) && *ptr != '-') {
        ecs_meta_error(ctx, ptr, "expected number, got %c", *ptr);
    }

    *value_out = strtol(ptr, NULL, 0);

    if (ptr[0] == '-') {
        ptr ++;
    } else 
    if (ptr[0] == '0' && ptr[1] == 'x') {
        ptr += 2;
    }

    while (isdigit(*ptr)) {
        ptr ++;
    }

    return skip_ws(ptr);
}

static
const char* parse_identifier(
    const char *ptr, 
    char *buff,
    char *params,
    ecs_meta_parse_ctx_t *ctx) 
{
    ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(buff != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ctx != NULL, ECS_INTERNAL_ERROR, NULL);

    char *bptr = buff, ch;

    if (params) {
        params[0] = '\0';
    }

    /* Ignore whitespaces */
    ptr = skip_ws(ptr);

    if (!isalpha(*ptr)) {
        ecs_meta_error(ctx, ptr, 
            "invalid identifier (starts with '%c')", *ptr);
    }

    while ((ch = *ptr) && !isspace(ch) && ch != ';' && ch != ',' && ch != ')' && ch != '>') {
        /* Type definitions can contain macro's or templates */
        if (ch == '(' || ch == '<') {
            if (!params) {
                ecs_meta_error(ctx, ptr, "unexpected %c", *ptr);
            }

            const char *end = skip_scope(ptr, ctx);
            ecs_os_strncpy(params, ptr, (ecs_size_t)(end - ptr));
            params[end - ptr] = '\0';

            ptr = end;
        } else {
            *bptr = ch;
            bptr ++;
            ptr ++;
        }
    }

    *bptr = '\0';

    if (!ch) {
        ecs_meta_error(ctx, ptr, "unexpected end of token");
    }

    return ptr;
}

static
const char * ecs_meta_open_scope(
    const char *ptr,
    ecs_meta_parse_ctx_t *ctx)    
{
    /* Skip initial whitespaces */
    ptr = skip_ws(ptr);

    /* Is this the start of the type definition? */
    if (ctx->decl == ptr) {
        if (*ptr != '{') {
            ecs_meta_error(ctx, ptr, "missing '{' in struct definition");     
        }

        ptr ++;
        ptr = skip_ws(ptr);
    }

    /* Is this the end of the type definition? */
    if (!*ptr) {
        ecs_meta_error(ctx, ptr, "missing '}' at end of struct definition");
    }   

    /* Is this the end of the type definition? */
    if (*ptr == '}') {
        ptr = skip_ws(ptr + 1);
        if (*ptr) {
            ecs_meta_error(ctx, ptr, 
                "stray characters after struct definition");
        }
        return NULL;
    }

    return ptr;
}

const char* ecs_meta_parse_constant(
    const char *ptr,
    ecs_meta_constant_t *token,
    ecs_meta_parse_ctx_t *ctx)
{    
    ptr = ecs_meta_open_scope(ptr, ctx);
    if (!ptr) {
        return NULL;
    }

    token->is_value_set = false;

    /* Parse token, constant identifier */
    ptr = parse_identifier(ptr, token->name, NULL, ctx);
    ptr = skip_ws(ptr);

    /* Explicit value assignment */
    if (*ptr == '=') {
        int64_t value = 0;
        ptr = parse_digit(ptr + 1, &value, ctx);
        token->value = value;
        token->is_value_set = true;
    }

    /* Expect a ',' or '}' */
    if (*ptr != ',' && *ptr != '}') {
        ecs_meta_error(ctx, ptr, "missing , after enum constant");
    }

    if (*ptr == ',') {
        return ptr + 1;
    } else {
        return ptr;
    }
}

static
const char* ecs_meta_parse_type(
    const char *ptr,
    ecs_meta_type_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    token->is_ptr = false;
    token->is_const = false;

    ptr = skip_ws(ptr);

    /* Parse token, expect type identifier or ECS_PROPERTY */
    ptr = parse_identifier(ptr, token->type, token->params, ctx);

    if (!strcmp(token->type, "ECS_PRIVATE")) {
        /* Members from this point are not stored in metadata */
        return NULL;
    }

    /* If token is const, set const flag and continue parsing type */
    if (!strcmp(token->type, "const")) {
        token->is_const = true;

        /* Parse type after const */
        ptr = parse_identifier(ptr + 1, token->type, token->params, ctx);
    }

    /* Check if type is a pointer */
    ptr = skip_ws(ptr);
    if (*ptr == '*') {
        token->is_ptr = true;
        ptr ++;
    }

    return ptr;
}

const char* ecs_meta_parse_member(
    const char *ptr,
    ecs_meta_member_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    ptr = ecs_meta_open_scope(ptr, ctx);
    if (!ptr) {
        return NULL;
    }

    token->count = 1;
    token->is_partial = false;

    /* Parse member type */
    ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
    if (!ptr) {
        /* If NULL is returned, parsing should stop */
        token->is_partial = true;
        return NULL;
    }

    /* Next token is the identifier */
    ptr = parse_identifier(ptr, token->name, NULL, ctx);

    /* Skip whitespace between member and [ or ; */
    ptr = skip_ws(ptr);

    /* Check if this is an array */
    char *array_start = strchr(token->name, '[');
    if (!array_start) {
        /* If the [ was separated by a space, it will not be parsed as part of
         * the name */
        if (*ptr == '[') {
            array_start = (char*)ptr; /* safe, will not be modified */
        }
    }

    if (array_start) {
        /* Check if the [ matches with a ] */
        char *array_end = strchr(array_start, ']');
        if (!array_end) {
            ecs_meta_error(ctx, ptr, "missing ']'");

        } else if (array_end - array_start == 0) {
            ecs_meta_error(ctx, ptr, "dynamic size arrays are not supported");
        }

        token->count = atoi(array_start + 1);

        if (array_start == ptr) {
            /* If [ was found after name, continue parsing after ] */
            ptr = array_end + 1;
        } else {
            /* If [ was fonud in name, replace it with 0 terminator */
            array_start[0] = '\0';
        }
    }

    /* Expect a ; */
    if (*ptr != ';') {
        ecs_meta_error(ctx, ptr, "missing ; after member declaration");
    }

    return ptr + 1;
}

void ecs_meta_parse_params(
    const char *ptr,
    ecs_meta_params_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    token->is_key_value = false;
    token->is_fixed_size = false;

    ptr = skip_ws(ptr);
    if (*ptr != '(' && *ptr != '<') {
        ecs_meta_error(ctx, ptr, 
            "expected '(' at start of collection definition");
    }

    ptr ++;

    /* Parse type identifier */
    ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
    ptr = skip_ws(ptr);
    
    /* If next token is a ',' the first type was a key type */
    if (*ptr == ',') {
        ptr = skip_ws(ptr + 1);
        
        if (isdigit(*ptr)) {
            int64_t value;
            ptr = parse_digit(ptr, &value, ctx);
            token->count = value;
            token->is_fixed_size = true;
        } else {
            token->key_type = token->type;

            /* Parse element type */
            ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
            ptr = skip_ws(ptr);

            token->is_key_value = true;
        }
    }

    if (*ptr != ')' && *ptr != '>') {
        ecs_meta_error(ctx, ptr, 
            "expected ')' at end of collection definition");
    }
}
