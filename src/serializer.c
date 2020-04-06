#include <flecs_components_meta.h>
#include "parser.h"

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module);

static
bool is_complex(ecs_type_op_kind_t kind) {
    if (kind == EcsOpPrimitive || 
        kind == EcsOpEnum ||
        kind == EcsOpBitmask)
    {
        return false;
    } else {
        return true;
    }
}

static
bool is_inline(ecs_type_op_kind_t kind) {
    if (!is_complex(kind) || kind == EcsOpPush)
    {
        return true;
    } else {
        return false;
    }
}

static
size_t ecs_get_primitive_size(
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
size_t ecs_get_primitive_alignment(
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
    EcsPrimitive *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
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
    EcsEnum *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{    
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

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_bitmask(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsBitmask *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{    
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

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_struct(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsStruct *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
    FlecsComponentsMetaImportHandles(*module);

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPush,
    };

    size_t size = 0;
    size_t alignment = 0;

    EcsMember *members = ecs_vector_first(type->members);
    int32_t i, count = ecs_vector_count(type->members);

    for (i = 0; i < count; i ++) {
        ops = serialize_type(world, members[i].type, ops, module);

        ecs_type_op_t *op = ecs_vector_last(ops, ecs_type_op_t);
        op->name = members[i].name;

        size_t member_size = op->size;
        uint8_t member_alignment = op->alignment;

        size = ECS_ALIGN(size, member_alignment);
        op->offset = size;

        size += member_size;      

        if (member_alignment > alignment) {
            alignment = member_alignment;
        }
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPop,
    };

    if (op_header) {
        op_header = ecs_vector_first(ops); /* Might have reallocd */
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = size,
            .alignment = alignment
        };
    }

    return ops;
}

static
ecs_vector_t* serialize_array(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsArray *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *handles)
{
    FlecsComponentsMetaImportHandles(*handles);

    EcsTypeSerializer *element_cache = ecs_get_ptr(world, type->element_type, EcsTypeSerializer);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    ecs_type_op_t *elem_op = ecs_vector_first(element_cache->ops);

    /* If element is inlined, don't add indirection to other cache */
    if (ecs_vector_count(element_cache->ops) == 1 && is_inline(elem_op->kind))
    {
        /* Serialize element op inline, and set count on first inserted op */
        uint32_t el_start = ecs_vector_count(ops);
        serialize_type(world, type->element_type, ops, handles);

        ecs_type_op_t *start = ecs_vector_get(ops, ecs_type_op_t, el_start);
        ecs_assert(start != NULL, ECS_INTERNAL_ERROR, NULL);

        start->count = type->count;
        start->size *= type->count;

        if (op_header) {
            *op_header = (ecs_type_op_t) {
                .kind = EcsOpHeader,
                .size = start->size,
                .alignment = start->alignment
            };            
        }
    } else {
        EcsType *element_type = ecs_get_ptr(world, type->element_type, EcsType);
        ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t){
            .kind = EcsOpArray, 
            .count = type->count,
            .size = element_type->size * type->count,
            .alignment = element_type->alignment,
            .is.collection = element_cache->ops
        };

        if (op_header) {
            *op_header = (ecs_type_op_t) {
                .kind = EcsOpHeader,
                .size = op->size,
                .alignment = op->alignment
            };            
        }        
    }

    return ops;
}

static
ecs_vector_t* serialize_vector(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsVector *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *handles)
{
    FlecsComponentsMetaImportHandles(*handles);

    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_vector_t*),
            .alignment = ECS_ALIGNOF(ecs_vector_t*)
        };         
    }

    EcsTypeSerializer *element_cache = ecs_get_ptr(world, type->element_type, EcsTypeSerializer);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    EcsType *element_type = ecs_get_ptr(world, type->element_type, EcsType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpVector, 
        .count = type->count,
        .size = sizeof(ecs_vector_t*),
        .alignment = ECS_ALIGNOF(ecs_vector_t*),
        .is.collection = element_cache->ops
    };

    return ops;
}

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
    FlecsComponentsMetaImportHandles(*module);

    EcsType *type = ecs_get_ptr(world, entity, EcsType);
    ecs_assert(type != NULL, ECS_INVALID_PARAMETER, NULL);

    switch(type->kind) {
    case EcsPrimitiveType: {
        EcsPrimitive *t = ecs_get_ptr(world, entity, EcsPrimitive);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_primitive(world, entity, t, ops, module);
    }

    case EcsEnumType: {
        EcsEnum *t = ecs_get_ptr(world, entity, EcsEnum);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_enum(world, entity, t, ops, module);
    }

    case EcsBitmaskType: {
        EcsBitmask *t = ecs_get_ptr(world, entity, EcsBitmask);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_bitmask(world, entity, t, ops, module);
    }    

    case EcsStructType: {
        EcsStruct *t = ecs_get_ptr(world, entity, EcsStruct);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_struct(world, entity, t, ops, module);
    }    

    case EcsArrayType: {
        EcsArray *t = ecs_get_ptr(world, entity, EcsArray);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_array(world, entity, t, ops, module);
    }

    case EcsVectorType: {
        EcsVector *t = ecs_get_ptr(world, entity, EcsVector);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_vector(world, entity, t, ops, module);
    }    

    default:
        break;
    }

    return NULL;
}

void EcsSetPrimitive(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsPrimitive, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(world, e, EcsTypeSerializer, {
            serialize_primitive(
                world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetEnum(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsEnum, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_enum(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetBitmask(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsBitmask, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_bitmask(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetStruct(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsStruct, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_struct(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetArray(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsArray, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_array(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetVector(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsVector, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_vector(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}
